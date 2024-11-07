package application;

import java.util.List;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;

public class Table {
    private List<String> columnNames;
    private List<List<String>> rows;

    public Table(List<String> columnNames, List<List<String>> rows) {
        this.columnNames = columnNames;
        this.rows = rows;
    }
    
    public Table(String tableStr) {
    	columnNames = FXCollections.observableArrayList();
    	rows = FXCollections.observableArrayList();

    	String[] tableColsAndRows = tableStr.split("\n");
    	String[] columns = tableColsAndRows[1].split(",");
    	for(String col : columns) {
    		columnNames.add(col);
    	}
    	for(int i = 2; i < tableColsAndRows.length; i++) {
    		List<String> row = FXCollections.observableArrayList();
    		String[] rowStr = tableColsAndRows[i].split(",");
    		for(String r : rowStr) {
    			row.add(r);
    		}
    		rows.add(row);
    	}
    	
    }

    public List<String> getColumnNames() {
        return columnNames;
    }

    public List<List<String>> getRows() {
        return rows;
    }
    
    public void showTable(TableView<ObservableList<String>> tableView ) {
    	Table table = this;
        // Brisanje postojećih kolona
        tableView.getColumns().clear();

        List<String> columnNames = table.getColumnNames();

        // Kreiranje kolona na osnovu meta podataka
        for (int i = 0; i < columnNames.size(); i++) {
            final int columnIndex = i;
            String columnName = columnNames.get(i);

            TableColumn<ObservableList<String>, String> column = new TableColumn<>(columnName);
            column.setCellValueFactory(param -> new SimpleStringProperty(param.getValue().get(columnIndex)));

            tableView.getColumns().add(column);
        }

        // Dodavanje podataka u TableView
        ObservableList<ObservableList<String>> data = FXCollections.observableArrayList();
        for (List<String> row : table.getRows()) {
            data.add(FXCollections.observableArrayList(row));
        }

        tableView.setItems(data);
    }
}
