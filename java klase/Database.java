package application;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

/**
 * @author Miletic
 * @version 1.0
 */
public class Database {
	
	String extension;
	String name;
	private boolean saved = true;
	/**
	 * constructor of a database using a name 
	 * @param dbName name of database that will be made
	 */
	public Database(String dbName) {
		name = dbName;
		extension = "mile";
	}
	/**
	 * set extension of database file from which database is loaded
	 * @param e type of extension of file that goes with database name
	 */
	public void setExtension(String e) {
		extension = e;
	}
	/**
	 * get saved status of database
	 * @return if database is saved
	 */
	public boolean getSaved() {
		return saved;
	}
	
	/**
	 * set saved status of database
	 * @param Saved set saved status of database
	 */
	public void setSaved(boolean Saved) {
		saved = Saved;
	}
	
	/**
	 * create a file where database will be stored
	 * @return true if a database is seccessfully created or false if not
	 */
	public boolean createDB() {
		return createDatabase(name);
	}
	
	/**
	 * open existing file and load a database
	 * @return true if opening is successfull
	 */
	public boolean openDB() {
		if(extension.equals(".sql")) {
			System.out.println("Otvoreno iz sql");
			return openDatabaseFromSQL(name);
		}else if(extension.equals("mile")) {
			return openDatabase(name);
		}return false;
	}
	
	/**
	 * save and store database
	 * @param andSqlFormat say if database need to be saved in SQL format besides saving in standarsd user's format
	 * @return true if saving is succesfull
	 */
	public boolean saveDB(boolean andSqlFormat) {
		return saveDatabase(name, andSqlFormat);
	}
	/**
	 * execute a query on database
	 * @param queryText text of queri for executing
	 * @return is a string that contains an information about result of executing a query
	 */
	public String executeQ(String queryText) {
		return executeQuery(name, queryText);
	}
	/**
	 * get table names of all curent existing tables in database
	 * @return string with tables names separated with '\n'
	 */
	public String tableNames() {
		return getTablesNames(name);
	}
	
	
	/**
	 * delete a table with given name from database 
	 * @param tableName name of table for deleting
	 */
	public void deleteTable(String tableName) {

		try {
            File file = new File(name+"copy.mile");
            BufferedReader br = new BufferedReader(new FileReader(file));
            StringBuilder stringBuilder = new StringBuilder();
            String line;
            int numOfSkip = 0;
            line = br.readLine();
            int numOfTables = Integer.parseInt(line);
            while ((line = br.readLine()) != null) {
            	String[] parts = line.split(" ");
            	if(parts[0].equals(tableName)) {
            		numOfSkip =Integer.parseInt(parts[1]) + 1 ;
            		numOfTables--;
            		continue;
            	}
            	if(numOfSkip > 0) {
            		numOfSkip--;
            		continue;
            	}
            	
                stringBuilder.append(line);
                stringBuilder.append("\n"); 
            }
            br.close();
            
            String wholeDocument = numOfTables + "\n" + stringBuilder.toString();
            
            BufferedWriter writer = new BufferedWriter(new FileWriter(name+"copy.mile"));
			writer.write(wholeDocument);
			writer.close();
            System.out.println(wholeDocument);

        } catch (IOException e) {
            System.out.println("Doslo je do greske prilikom citanja datoteke.");
            e.printStackTrace();
        }
	}
	
	
	/**
	 * open database from SQL formated file
	 * @param databaseName name of database that needs to be opened
	 * @return if database is successfully openede
	 */
	private static boolean openDatabaseFromSQL(String databaseName) {
		
		try {
			//Main.db = new Database(databaseName);
			createDatabase(databaseName);
			openDatabase(databaseName);
            File file = new File(databaseName+".sql");
            BufferedReader br = new BufferedReader(new FileReader(file));
            StringBuilder stringBuilder = new StringBuilder();
            String line;
            while ((line = br.readLine()) != null) {
                stringBuilder.append(line);
                stringBuilder.append(" "); 
            }
            br.close();
            String wholeDocument = stringBuilder.toString();
            
    		wholeDocument = wholeDocument.replace(" varchar(255)","");
    		wholeDocument = wholeDocument.replace('\'','\"');
    		wholeDocument = wholeDocument.replace("( "," (");
    		wholeDocument = wholeDocument.replace(" )",")");

    		
    		String[] parts = wholeDocument.split(";");
    		for(String s :parts) {
    			System.out.println(s);
                executeQuery(databaseName, s); 
    		}
    		saveDatabase(databaseName, false);

        } catch (IOException e) {
            //System.out.println("Došlo je do greške prilikom čitanja datoteke.");
            e.printStackTrace();
            return false;
        }
		return true;
	}
	
	/**
	 * create database
	 * @param databaseName
	 * @return if database is successfully created
	 */
	private static native boolean createDatabase(String databaseName);
	
	/**
	 * open database
	 * @param databaseName
	 * @return if database is successfully opened
	 */
	private static native boolean openDatabase(String databaseName);
	
	/**
	 * save database
	 * @param databaseName
	 * @return if database is successfully saved
	 */
	private static native boolean saveDatabase(String databaseName, boolean andSqlFormat);
	/**
	 * execute query on database
	 * @param databaseName
	 * @return result from xecuting query
	 */
	private static native String executeQuery(String databaseName, String queryText);
	/**
	 * get list of tables in database
	 * @param databaseName
	 * @return table names separated with '\n'
	 */
	private static native String getTablesNames(String databaseName);
	
	

}
