package application;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.animation.KeyFrame;

import javafx.animation.Timeline;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TableView;
import javafx.scene.control.TextArea;
import javafx.util.Duration;

public class ControllerMain implements Initializable{
	
	@FXML
	private Button executeButton;
	
	@FXML
	private Button deleteButton;
	
	@FXML
	private Label resultLabel;
	
	@FXML 
	private Label saveLabel;
	
	@FXML
	private TextArea queryTextArea;
	
	@FXML
	private ListView<String> tableNamesList;
	
	@FXML
    private TableView<ObservableList<String>> tableViewWholeTable;

	@FXML
    private TableView<ObservableList<String>> tableViewQueryTable;
	
	private String[] tables;
	
	

	
	@Override
	public void initialize(URL arg0, ResourceBundle arg1) {
		tableNamesList.getItems().clear();
		initTableNames();
		tableNamesList.getItems().addAll(tables);
		
		tableNamesList.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<String>() {

			@Override
			public void changed(ObservableValue<? extends String> arg0, String arg1, String arg2) {
				
				String selectedTableName = tableNamesList.getSelectionModel().getSelectedItem();
				
				String query = "select * from " + selectedTableName;
				
				String tableText = Main.db.executeQ(query);
				
				Table t = new Table(tableText);
				t.showTable(tableViewWholeTable);
				
				
			}	
		});
		tableNamesList.getSelectionModel().selectFirst();
		
	}
	private void initTableNames() {
		String tableNames = Main.db.tableNames();
		tables = tableNames.split("\n");
	}
	
	public void deleteButtonMethod(ActionEvent event) {
		String selectedTableName = tableNamesList.getSelectionModel().getSelectedItem();
		Main.db.deleteTable(selectedTableName);
		initialize(null, null);
	}
    
	public void executeQuery(ActionEvent event) {
		if(queryTextArea.getText().equals("")) return;
		String queryText = queryTextArea.getText().replace('\n', ' ');
		String queryResult = Main.db.executeQ(queryText);
		
		String[] parts = queryResult.split("\n");
		if(parts[0].equals("Table")) {
			Table t = new Table(queryResult);
			t.showTable(tableViewQueryTable);
			resultLabel.setText("Rezultat:");

		}else {
			String[] partsOfResult = queryResult.split(" ");
			if(!partsOfResult[0].equals("Greska!")) {
				Main.db.setSaved(false);
				initialize(null, null);
			}
			resultLabel.setText(queryResult);

		}
		
	}
	public void save(ActionEvent event) {
		Main.db.setSaved(true);
		Main.db.saveDB(false);
		
		saveLabel.setText("Uspesno sacuvavanje");
		saveLabel.setVisible(true);
        Timeline timeline = new Timeline(new KeyFrame(
                Duration.seconds(3),
                ae -> saveLabel.setVisible(false)
        ));
        timeline.play();
	}
	public void saveSQL(ActionEvent event) {
		Main.db.saveDB(true);
		saveLabel.setText("Uspesno sacuvavanje u SQL formatu");
		saveLabel.setVisible(true);
        Timeline timeline = new Timeline(new KeyFrame(
                Duration.seconds(3),
                ae -> saveLabel.setVisible(false)
        ));
        timeline.play();
	}
}
