//
// Created by Miletic on 4/18/2024.
//

#include "Database.h"
//pomocne metode
bool Database::hasTableName(const string &tableName) {
    return std::any_of(tables.begin(), tables.end(), [&tableName](Table* t) {
        return t->getTableName() == tableName;
    });
}

Table *Database::getTableByName(const string &tableName) {
    if(!hasTableName(tableName)) return nullptr;
    Table *table = nullptr;
    for(Table* t : tables){
        if(t->getTableName() == tableName){ table = t; break; }
    }
    return table;
}

//export import save
void Database::exportDBinSQL(ostream &file) const {
    for(Table* t: tables){
        file << "CREATE TABLE " << t->getTableName() <<"(\n";
        int j = 0;
        for(const Column& c: t->getColumns()){
            file << c.getColumnName() << " varchar(255)";
            if(j != t->getNumOfColumns() - 1 ) file << ",";
            file << endl;
            j++;
        }
        file << ");\n";
        string colStr = t->columnsToText();
        for(int i = 0; i < t->getNumOfRows(); i++){
            file << "INSERT INTO " << t->getTableName() << " (" << colStr << ")\n";
            file << "VALUES (" << t->ithRowToText(i) << ");\n";
        }

    }
}

void Database::exportDBinMyFormat(ofstream &file) const {
    file << tables.size() << endl;
    for(Table *table: tables){
        file << table->getTableName() << " " <<table->getNumOfRows() << " " << table->getNumOfColumns() << endl;
        int n = table->getNumOfColumns();
        for(int i = 0; i < n;i++){
            file << table->getColumns()[i].getColumnName();
            if(i != n-1) file << " "; // ili , umesto razmaka
        }
        file << endl;
        int m = table->getNumOfRows();
        for(int j = 0; j < m; j++){
            for(int i = 0; i < n;i++){
                file << table->getColumns()[i].getValues()[j];
                if(i != n-1) file << " "; // ili , umesto razmaka
            }
            file << endl;
        }
    }
}

Database *Database::importDB(istream &file) {

    auto* db = new Database();
    int numOfTable;
    file >> numOfTable;
    for(int t = 0; t < numOfTable; t++){
        string tableName;
        int numOfRows, numOfCols;
        file >> tableName >> numOfRows >> numOfCols;
        vector<string> columnNames;
        for(int c = 0 ; c < numOfCols; c++){
            string colName;
            file >> colName;
            columnNames.push_back(colName);
        }
        db->createTable(tableName, columnNames);
        vector<vector<string>> values;
        for(int r = 0; r < numOfRows; r++){
            vector<string> rowValues;
            for(int c = 0 ; c < numOfCols; c++){
                string value;
                file >> value;
                rowValues.push_back(value);
            }
            values.push_back(rowValues);
        }
        db->insertInto(tableName, columnNames, values);

    }
    return db;
}

string Database::saveDB(string file) {

    //cout << "Unesite ime fajla u koji da se sacuva baza:\n";
    //string file;
    //getline(std::cin, file);
    ofstream outFile(file);
    if(!outFile.is_open()){
        cout << "Neuspesno otvaranje fajla: " + file + "\n";
        return "Neuspesno otvaranje fajla.\n";
    }
    regex sqlReg(R"(\w+\.sql)");
    smatch m;
    if(regex_search(file, m, sqlReg)){
        exportDBinSQL(outFile);
        cout << "Vasa baza je uspesno sacuvana u sql formatu.\n";
        return "Vasa baza je uspesno sacuvana u sql formatu.\n";
    }
    else{
        exportDBinMyFormat(outFile);
        cout << "Vasa baza je uspesno sacuvana i moze se kasnije ponovo otvoriti.\n";
    }
    return "Vasa baza je uspesno sacuvana i moze se kasnije ponovo otvoriti.\n";
}

//upiti
void Database::showTables() const {
    std::cout << "Tables:\n";
    for(Table* t: tables){
        std::cout << t->getTableName() << " (" << t->columnsToText() << ")"<< endl;
    }
}

string Database::showTablesText() const {
    string rez = "";
    for(Table* t: tables){
        rez += t->getTableName() + "\n";
    }
    return rez;
}


void Database::createTable(const string &tableName, const vector<string> &columns) {
    if(hasTableName(tableName)) throw EAlreadyExistingTableName(tableName);
    auto *t = new Table(tableName);
    t->setColumns(columns);
    tables.push_back(t);
}

void Database::insertInto(const string &tableName, const vector<string> &columns, const vector<vector<string>>& values) {
    Table *table = getTableByName(tableName);
    if(!table) throw ENonExistingTable(tableName);
    for(const auto& vals:values)
        if(columns.size() != vals.size()) throw ENonCompatibleNumOfColsAndVals();
    if(table->getNumOfColumns() != columns.size() ) throw ENotCompatibleRow(tableName);
    if(!table->containsAllColums(columns)) throw ENotCompatibleColumns(tableName);
    int n = (int)table->getColumns().size();
    for(int i = 0; i < n; i++){
        if(table->getColumns()[i].getColumnName() != columns[i]) throw ENotCompatibleColumns(tableName);
    }
    for(const auto& vals:values)
        table->addRow(vals);
}

int Database::deleteFrom(Table *table, vector<Condition *> &conds) {
    for(Condition* c : conds) if(!table->hasColumn(c->getCondsColumnName())) throw ENonExistingColumn(c->getCondsColumnName(), table->getTableName());

    for(Condition* c : conds){
        for(int i = 0; i < table->getNumOfColumns(); i++)
            if(c->getCondsColumnName() == table->getColumns()[i].getColumnName()) {
                c->setIndex(i);
                break;
            }
    }
    int numOfDeleted = 0;
    for(int i = 0; i < table->getNumOfRows(); i++){
        bool del = true;
        for(Condition* c :conds){                             //index kolone za uslov      index vrste
            if(!c->doesItFulfillCondition(table->getColumns()[c->getIndex()].getValues()[i])) {
                del = false;
                break;
            }
        }
        if(del){
            table->deleteIthRow(i);
            numOfDeleted++;
            i--;
        }

    }
    return numOfDeleted;
}

int Database::update(const string& tableName, vector<Change> &changes, const vector<Condition *> &conds) {
    Table *table = getTableByName(tableName);
    if(!table) throw ENonExistingTable(tableName);

    for(Condition* c : conds) if(!table->hasColumn(c->getCondsColumnName())) throw ENonExistingColumn(c->getCondsColumnName(), tableName);
    for(const Change& c : changes) if(!table->hasColumn(c.getChangeColumnName())) throw ENonExistingColumn(c.getChangeColumnName(), tableName);

    //dodeljivanje svakom uslovu indexa kolona
    for(Condition* c : conds){
        for(int i = 0; i < table->getNumOfColumns(); i++)
            if(c->getCondsColumnName() == table->getColumns()[i].getColumnName()) {
                c->setIndex(i);
                break;
            }
    }

    //dodeljivanje svakoj promeni indexa kolone
    for(Change& c : changes){
        for(int i = 0; i < table->getNumOfColumns(); i++)
            if(c.getChangeColumnName() == table->getColumns()[i].getColumnName()) {
                c.setIndex(i);
                break;
            }
    }
    int numOfUpd = 0;
    for(int i = 0; i < table->getNumOfRows(); i++){
        bool upd = true;
        for(Condition* c :conds){                             //index kolone za uslov      index vrste
            if(!c->doesItFulfillCondition(table->getColumns()[c->getIndex()].getValues()[i])) {
                upd = false;
                break;
            }
        }
        if(upd){
            numOfUpd++;
            for(const Change& c: changes){
                table->getColumns()[c.getIndex()].setIthRow(c.getNewValue(), i);
            }
        }
    }
    return numOfUpd;
}

string Database::select(Table *table, const vector<string> &selectedColumns, const vector<Condition *> &conds, vector<OrderBy> *order) {
    Table pomocnaSaProbranim(table->getTableName());
    if(selectedColumns[0] != "*")
        if(!table->containsAllColums(selectedColumns)) throw ENotCompatibleColumns(table->getTableName());
    for(Condition* c : conds) if(!table->hasColumn(c->getCondsColumnName())) throw ENonExistingColumn(c->getCondsColumnName(), table->getTableName());
    if(order){
        for(OrderBy o : *order) if(!table->hasColumn(o.getColumnName())) throw ENonExistingColumn(o.getColumnName(), table->getTableName());
    }

    if(selectedColumns[0] == "*") pomocnaSaProbranim.setColumns(table->getNamesOfColumns());
    else pomocnaSaProbranim.setColumns(selectedColumns);

    Table pomocnaSaSvim(*table);

    for(Condition* c : conds){
        for(int i = 0; i < table->getNumOfColumns(); i++)
            if(c->getCondsColumnName() == table->getColumns()[i].getColumnName()) {
                c->setIndex(i);
                break;
            }
    }
    for(int i = 0; i < pomocnaSaSvim.getNumOfRows(); i++){
        for(Condition* c :conds){                             //index kolone za uslov      index vrste
            if(!c->doesItFulfillCondition(pomocnaSaSvim.getColumns()[c->getIndex()].getValues()[i])) {
                pomocnaSaSvim.deleteIthRow(i);
                i--;
                break;
            }
        }
    }

    if(order) pomocnaSaSvim.orderByTable(order);

    //pomocna sa svim tabela je profiltrirana sad
    for(Column& col : pomocnaSaProbranim.getColumns()){
        for( Column& colAll : pomocnaSaSvim.getColumns()){
            if(col.getColumnName() == colAll.getColumnName()){
                col.setValues(colAll.getValues());
                break;
            }
        }
    }
    Table::printTable(pomocnaSaProbranim);
    return pomocnaSaProbranim.tableToText();

}

void Database::dropTable(const string &tableName) {
    if(!hasTableName(tableName)) throw ENonExistingTable(tableName);
    tables.erase(std::remove_if(tables.begin(), tables.end(), [tableName](Table* t) { return t->getTableName() == tableName; }), tables.end());
}

//regex stringovi
const string Database::createTableString = R"(\s*(?:CREATE TABLE|create table)\s+(\w+)\s*\(\s*((?:\w+(?:,\s*)?)+)\);?)";
const string Database::dropTableString = R"(\s*(?:DROP TABLE|drop table)\s+(\w+)\s*;?)";
const string Database::insertIntoString = R"(\s*(?:INSERT INTO|insert into)\s+(\w+)\s*\(\s*((?:\w+(?:\s*,\s*)?)+)\)\s+(?:VALUES|values)\s*((?:\((?:\"\w+\"(?:\s*,\s*)?)+\)(?:\s*,\s*)?)+);?)";
const string Database::updateString = R"(\s*(?:UPDATE|update)\s+(\w+)\s+(?:SET|set)\s+((?:\w+\s*=\s*"\w+\"\s*,?\s*)+)\s+(?:WHERE|where)\s+((?:\w+\s*(?:=|<>)\s*"\w+\"\s*(?:AND|and)?\s*)+);?)";
const string Database::deleteString = R"(\s*(?:DELETE FROM|delete from)\s+(\w+)\s+(?:WHERE|where)\s+((?:\w+\s*(?:=|<>)\s*"\w+\"\s*(?:AND|and)?\s*)+);?)";
const string Database::selectString = R"(\s*(?:SELECT|select)\s+(\*|(?:(?:\w+(?:,\s*)?)+))\s+(?:FROM|from)\s+(\w+)(?:\s+(?:WHERE|where)\s+((?:\w+\s*(?:=|<>)\s*"\w+\"\s*(?:AND|and)?\s*)+))?\s*(?:(?:ORDER BY|order by)\s+((?:\w+\s*(?:asc|desc|ASC|DESC)?(?:\s*,\s*)?)+))?;?)";
const string Database::selectJoinString = R"(\s*(?:SELECT|select)\s+(\*|(?:(?:(?:\w+\.)?\w+(?:,\s*)?)+))\s+(?:FROM|from)\s+(\w+(?:\s+(?:AS|as)\s+\w+)?(?:(?:\s+INER JOIN|\s+iner join)\s+\w+(?:\s+(?:AS|as)\s+\w+)?\s+(?:ON|on)\s+(?:\w+\.)?\w+\s+=\s+(?:\w+\.)?\w+)?)(?:\s+(?:WHERE|where)\s+((?:(?:\w+\.)?\w+\s*(?:=|<>)\s*"\w+\"\s*(?:AND|and)?\s*)+))?\s*(?:(?:ORDER BY|order by)\s+((?:(?:\w+\.)?\w+\s*(?:asc|desc|ASC|DESC)?(?:\s*,\s*)?)+))?;?)";
const string Database::showTablesString = R"(\s*(?:SHOW TABLES|show tables)\s*;?)";

//parse metode
vector<Condition *> Database::parseConditions(const string &strConds) { //uslovi u formatu t = "1" and r <> "r"

    smatch matches;
    vector<Condition*> conds;
    regex oneCond(R"(\w+\s*(?:=|<>)\s*\"\w+\")");
    regex partsOfCond(R"((\w+)\s*((?:=|<>))\s*\"(\w+)\")");

    sregex_iterator currentMatch(strConds.begin(), strConds.end(), oneCond);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
        smatch match = *currentMatch;
        //cout << match.str() << endl;
        smatch partMatches;
        string s = match.str();
        regex_search(s,partMatches, partsOfCond);
        if(partMatches[2].str() == "="){
            conds.emplace_back(new Equals(partMatches[1].str(), partMatches[3].str()));
        }else{
            conds.emplace_back(new NotEquals(partMatches[1].str(), partMatches[3].str()));
        }
        currentMatch++;
    }
    return conds;
}

vector<string> Database::parseColumns(const string &strCols) {
    smatch matches;
    vector<string> columns;
    regex col(R"((?:\*|\w+))");
    sregex_iterator currentMatch(strCols.begin(), strCols.end(), col);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
        smatch match = *currentMatch;
        //cout << match.str() << endl;
        columns.push_back(match.str());

        currentMatch++;
    }
    return columns;
}

vector<Change> Database::parseChanges(const string &strChanges) {

    smatch matches;
    vector<Change> conds;
    regex oneCond(R"(\w+\s*=\s*\"\w+\")");
    regex partsOfCond(R"((\w+)\s*=\s*\"(\w+)\")");

    sregex_iterator currentMatch(strChanges.begin(), strChanges.end(), oneCond);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
        smatch match = *currentMatch;
        smatch partMatches;
        string s = match.str();
        regex_search(s,partMatches, partsOfCond);
        conds.emplace_back(partMatches[1].str(), partMatches[2].str());

        currentMatch++;
    }
    return conds;
}

vector<vector<string>> Database::parseNewValues(const string &strNewVals) {
    vector<vector<string>> skupVrednosti;
    smatch skupovi;
    regex forSets(R"(\([^\(]+\))");
    regex forEachSet (R"(\w+)");
    sregex_iterator currentMatch1(strNewVals.begin(), strNewVals.end(), forSets);
    sregex_iterator lastMatch1;
    while(currentMatch1 != lastMatch1){//izdvajam svaku zagradu posebno
        smatch match = *currentMatch1;
        string s = match.str();
        sregex_iterator currentMatch2(s.begin(), s.end(), forEachSet);
        sregex_iterator lastMatch2;
        vector<string> v;
        while(currentMatch2 != lastMatch2) { //iz svake zagrade izvlacim sve vrednosti
            smatch vrednosti = *currentMatch2;
            string val = vrednosti.str();
            v.push_back(val);
            currentMatch2++;
        }
        skupVrednosti.push_back(v);
        currentMatch1++;
    }
    return skupVrednosti;
}

vector<OrderBy> *Database::parseOrders(const string &strOrders) { // govna za order by u formatu col1 asc, col2, col3 desc itd

    if(strOrders == "") return nullptr;

    vector<OrderBy> * ords = new vector<OrderBy>;

    string ordReg = R"(\w+(?:\s*(?:asc|desc|ASC|DESC)?))";
    regex order(ordReg);
    sregex_iterator currentMatch(strOrders.begin(), strOrders.end(), order);
    sregex_iterator lastMatch;
    while(currentMatch != lastMatch){
        smatch match = *currentMatch;
        //cout << match.str() << endl;
        string o = match.str();

        smatch parts;
        regex p(R"((\w+)\s*((?:asc|desc|ASC|DESC)?))");
        regex_search(o,parts, p);
        if(parts[2].str() == "" || parts[2].str() == "asc" || parts[2].str() == "ASC"){
            ords->emplace_back(OrderBy(parts[1], OrderSort::ASC));
        }else{
            ords->emplace_back(OrderBy(parts[1], OrderSort::DESC));
        }
        currentMatch++;
    }

    return ords;
}

//glavna
string Database::query(const string &line) {

    string returnString;

    smatch match;

    regex createT(createTableString);
    regex dropT(dropTableString);
    regex insertIntoT(insertIntoString);
    regex updateT(updateString);
    regex deleteT(deleteString);
    regex selectT(selectString);
    regex selectJoinT(selectJoinString);
    regex showT(showTablesString);
    try{
        if(regex_search(line, match, createT)){// create table upit
            string tableName = match[1].str();   //ime nove tabele
            string columns = match[2].str();     //imena kolona odvojena zarezom **implementiran parse**
            auto Columns = parseColumns(columns);
            createTable(tableName, Columns); //izvrsavanje upita
            returnString = "Successfully created table: " + tableName + "\n";

        }else if(regex_search(line, match, dropT)){//drop table upit
            string tableName = match[1].str();  //ime tabele **implementiran parse**
            dropTable(tableName); //izvrsavanje upita
            returnString = "Successfully droped table: " + tableName + "\n";

        }else if(regex_search(line, match, insertIntoT)){//insert into upit
            string tableName = match[1].str();  //ime tabele
            string columns = match[2].str();    //imena kolona odvojena zarezima                                **implementiran parse**
            string values = match[3].str();     //nove vrednosti u formatu ("val1", "val2"), ("val3", "val4")   **implementiran parse**
            auto Columns = parseColumns(columns);
            auto Values = parseNewValues(values);
            insertInto(tableName, Columns, Values); //izvrsavanje upita
            returnString = "Successfully inserted " + to_string(Values.size()) +" row" + ((Values.size() > 1)?"s":"") + " in table " + tableName + "\n";

        }else if(regex_search(line, match, updateT)){//update upit
            string tableName = match[1].str();  //ime tabele
            string newValues = match[2].str();  //vrednosti koje se update-ju u formatu  k1 = "3", k2 = "5" **implementiran parse**
            string conditions = match[3].str();  //uslovi u formatu t = "1" and r <> "r"                    **implementiran parse**
            auto Changes = parseChanges(newValues);
            auto Conditions = parseConditions(conditions);
            int numOfUpdated = update(tableName, Changes, Conditions); //izvrsavanje upita

            returnString =  "Successfully updated " + to_string(numOfUpdated) + " row" + ((numOfUpdated > 1)?"s":"") +" from table " + tableName + "\n";
        }else if(regex_search(line, match, deleteT)){//delete upit
            string tableName = match[1].str();  //ime tabele
            string conditions = match[2].str();  //uslovi u formatu t = "1" and r <> "r"  **implementiran parse**
            auto Conditions = parseConditions(conditions);
            Table* table = getTableByName(tableName);
            if(!table) throw ENonExistingTable(tableName);
            int numOfDeleted = deleteFrom(table, Conditions); //izvrsavanje upita
            returnString = "Successfully deleted " + to_string(numOfDeleted) + " row" + ((numOfDeleted > 1)?"s":"") + " from table " + tableName +"\n";
        }else if(regex_search(line, match, selectT)){//select upit
            string columns = match[1].str();     //imena kolona odvojena zarezom          **implementiran parse**
            string tableName = match[2].str();  //ime tabele
            string conditions = match[3].str();  //uslovi u formatu t = "1" and r <> "r"  **implementiran parse**
            string orders = match[4].str();  // govna za order by u formatu col1 asc, col2, col3 desc itd **implementiran parse**

            Table* table = getTableByName(tableName);
            if(!table) throw ENonExistingTable(tableName);
            auto Conditions = parseConditions(conditions);
            auto Columns = parseColumns(columns);
            auto ords = parseOrders(orders);

            returnString =  select(table, Columns, Conditions, ords); //izvrsavanje upita

        }else if(regex_search(line, match, selectJoinT)){//select upit sa opcijom join-a
            string columns = match[1].str();     //imena kolona odvojena zarezom s tim sto mogu biti r.r, f.f   *iskorisceno
            string tablesNames = match[2].str();  //imena tabela u formatu firma as f iner join posao as p on f.id = p.id *iskorisceno
            string conditions = match[3].str();  //uslovi u formatu t.t = "1" and r.r <> "r"  *iskorisceno
            string orders = match[4].str(); // // govna za order by u formatu c.col1 asc, col2, col3 desc itd
            //cout << "prepoznat iner join\n";
            string joinTablesAndOnPart = R"((\w+)(?:\s+(?:AS|as)\s+(\w+))?(?:(?:\s+INER JOIN|\s+iner join)\s+(\w+)(?:\s+(?:AS|as)\s+(\w+))?\s+(?:ON|on)\s+(?:(\w+)\.)?(\w+)\s+=\s+(?:(\w+)\.)?(\w+))?)";
            //                              -----                  -----                                     -----                  -----                    -----    -----          -----    -----
            //                             ime 1. tabele          nadimak 1. tabele                         ime 2. tabele          nadimak 2. tabele     nad 1. tab  ime kolone   nad 2. tab  ime kolone
            smatch deloviJoina;
            regex r(joinTablesAndOnPart);
            regex_search(tablesNames, deloviJoina, r);

            string table1Name = deloviJoina[1].str();                       /////
            string table1Nick = deloviJoina[2].str();                       /////
            string table2Name = deloviJoina[3].str();                       /////
            string table2Nick = deloviJoina[4].str();                       /////
            string nickWith1stCol = deloviJoina[5].str();                   /////
            string column1Name = deloviJoina[6].str();                      /////
            string nickWith2ndCol = deloviJoina[7].str();                   /////
            string column2Name = deloviJoina[8].str();                      /////
            string fullCol1Name = (nickWith1stCol==""?"":nickWith1stCol + ".") + column1Name;
            string fullCol2Name = (nickWith2ndCol==""?"":nickWith2ndCol + ".") + column2Name;

            vector<string> punaImenaKolona;                                 /////
            vector<string> prefikis;                                        /////
            vector<string> imenaKolona;                                     /////

            string izdvojPrefiksiImeIzKolone = R"((?:(\w+)\.)?(\w+))";
            smatch matchKolona;
            regex regexKolona(izdvojPrefiksiImeIzKolone);

            sregex_iterator currentMatch(columns.begin(), columns.end(), regexKolona);
            sregex_iterator lastMatch;
            while(currentMatch != lastMatch){
                matchKolona = *currentMatch;
                punaImenaKolona.push_back(matchKolona[0].str());
                prefikis.push_back(matchKolona[1].str());
                imenaKolona.push_back(matchKolona[2].str());
                currentMatch++;
            }

            string izdvajanjeUslova = R"(((?:\w+\.)?\w+)\s*((?:=|<>))\s*"(\w+)\")";
            //                           ---------------   ----------    -----
            //                            puno ime kolone     znak       vrednost
            smatch matchUslova;
            regex zaUslove(izdvajanjeUslova);
            sregex_iterator currentMatch1(conditions.begin(), conditions.end(), zaUslove);
            sregex_iterator lastMatch1;
            vector<Condition*> uslovi;                                      /////
            while(currentMatch1 != lastMatch1){
                matchUslova = *currentMatch1;

                if(matchUslova[2].str() == "="){
                    uslovi.emplace_back(new Equals(matchUslova[1].str(), matchUslova[3].str()));
                }else{
                    uslovi.emplace_back(new NotEquals(matchUslova[1].str(), matchUslova[3].str()));
                }
                currentMatch1++;
            }

            vector<OrderBy>* orderss = new vector<OrderBy>;                 /////
            string izdvajanjeOrdera = R"(((?:\w+\.)?\w+)\s*((?:asc|desc|ASC|DESC))?)";
            //                           ---------------   -----------------------
            //                           puno ime kolone         vrsta ordera
            smatch matchOrdera;
            regex zaOrdere(izdvajanjeOrdera);
            sregex_iterator currentMatch2(orders.begin(), orders.end(), zaOrdere);
            sregex_iterator lastMatch2;
            while(currentMatch2 != lastMatch2){
                matchOrdera = *currentMatch2;
                if(matchOrdera[2].str() == "" || matchOrdera[2].str() == "asc" || matchOrdera[2].str() == "ASC"){
                    orderss->emplace_back(matchOrdera[1], OrderSort::ASC);
                }else{
                    orderss->emplace_back(matchOrdera[1], OrderSort::DESC);
                }
                currentMatch2++;
            }
            cout << columns <<endl;
            cout << tablesNames << endl;
            cout << conditions << endl;
            cout << orders << endl;

            //sad jos samo spojiti 2 tabele u jednu i pozvati select za tu tabelu
            Table *t1 = getTableByName(table1Name);
            if(!t1) throw ENonExistingTable(table1Name);
            Table *t2 = getTableByName(table2Name);
            if(!t2) throw ENonExistingTable(table2Name);
            if(table1Nick == table2Nick || nickWith1stCol == nickWith2ndCol ||
                (nickWith1stCol != table1Nick && nickWith1stCol != table2Nick) ||
               (nickWith2ndCol != table1Nick && nickWith2ndCol != table2Nick)) throw ENotOkNicknames();
            int n = imenaKolona.size();
            for(int i = 0; i < n; i++){
                if(prefikis[i] == "" && t1->hasColumn(imenaKolona[i]) && t2->hasColumn(imenaKolona[i])) throw EAmbiguousColumnName(imenaKolona[i]);
                if(prefikis[i] != "" && prefikis[i] == table1Nick && !t1->hasColumn(imenaKolona[i])) throw ENonExistingColumn(imenaKolona[i], table1Name);
                if(prefikis[i] != "" && prefikis[i] == table2Nick && !t2->hasColumn(imenaKolona[i])) throw ENonExistingColumn(imenaKolona[i], table2Name);
            }
            Table t1copy(*t1);
            int indexKolonePrveTabele = t1copy.getColumnIndexByName(column1Name);
            Table t2copy(*t2);
            int indexKoloneDrugeTabele = t2copy.getColumnIndexByName(column2Name);
            for(int i = 0; i < n; i++){
                if(prefikis[i] != "" && prefikis[i] == table1Nick && t1copy.hasColumn(imenaKolona[i])){
                    int index = t1copy.getColumnIndexByName(imenaKolona[i]);
                    t1copy.getColumns()[index].setColumnName(punaImenaKolona[i]);
                }
                if(prefikis[i] != "" && prefikis[i] == table2Nick && t2copy.hasColumn(imenaKolona[i])){
                    int index = t2copy.getColumnIndexByName(imenaKolona[i]);
                    t2copy.getColumns()[index].setColumnName(punaImenaKolona[i]);
                }
            }
            Table finalTable("final");
            vector<string> allColumns;
            for(auto c : t1copy.getColumns()){
                allColumns.push_back(c.getColumnName());
            }
            for(auto c : t2copy.getColumns()){
                allColumns.push_back(c.getColumnName());
            }
            finalTable.setColumns(allColumns);
            for(int i = 0 ; i < t1copy.getNumOfRows(); i++){
                for(int j = 0; j < t2copy.getNumOfRows(); j++){
                    if(t1copy.getColumns()[indexKolonePrveTabele].getValues()[i] == t2copy.getColumns()[indexKoloneDrugeTabele].getValues()[j]){
                        vector <string> row = t1copy.getIthRow(i);
                        vector <string> rowt2 = t2copy.getIthRow(j);
                        row.insert(row.end(), rowt2.begin(), rowt2.end());
                        finalTable.addRow(row);
                    }
                }
            }
            returnString = select(&finalTable, punaImenaKolona, uslovi, orderss);

        }else if(regex_search(line, match, showT)){//show tables upit
            showTables(); //izvrsavanje upita
            returnString = showTablesText();
        }else{//neregularan upit
            throw EIllegalQuery();
        }
    }
    catch(exception& e){
        returnString = e.what();
    }
    cout << "--------------------------------\n";
    cout << "returnString:\n";
    cout << returnString;
    cout << "--------------------------------\n";
    return returnString;
}


