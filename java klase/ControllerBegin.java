package application;

import java.io.IOException;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.RadioButton;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.control.Alert.AlertType;
import javafx.stage.Modality;
import javafx.stage.Stage;


public class ControllerBegin {
	
	static {
        System.loadLibrary("bibliotekaZaBazu");
    }

	
	private Stage stage;
	private Scene scene;
	private Parent root;
	
	
	@FXML
	ToggleGroup group1;
	
	@FXML
	private TextField dbNameTextField;
	
	@FXML
	private Button startingButton;
	
	@FXML
	private RadioButton rbCreate, rbOpen;
	

	
	public void switchToSceneMain(ActionEvent event) throws IOException {
		
		stage = (Stage)((Node)event.getSource()).getScene().getWindow();
		
		if(!initiliazee(stage)) {			
			return;
		}	
		FXMLLoader loader = new FXMLLoader(getClass().getResource("Main.fxml"));
		root = loader.load();
		scene = new Scene(root); 
		scene.setUserData("MainScena");
		stage.setScene(scene);   
		stage.setMaximized(true); 
		
		stage.show();	 
	}
	
	
		 
	public boolean initiliazee(Stage stage) {
		String databaseName = dbNameTextField.getText();

		String extension = getExtension(databaseName);
		databaseName = removeFileExtension(databaseName);

		Main.db = new Database(databaseName);
		Main.db.setExtension(extension);
		
		if(rbCreate.isSelected()) {
			if(databaseName.equals("")) { 
				throwAlert(stage,"Unesite ispravno ime.");
				return false;
			}
			if(Main.db.openDB()) {
				throwAlert(stage, existingDBName(databaseName));
				return false;
			}
			Main.db.createDB();
		}
		else if(rbOpen.isSelected()) {
			if(databaseName.equals("")) { 
				throwAlert(stage,"Unesite ispravno ime.");
				return false;
			}
			if(!Main.db.openDB()) {
				throwAlert(stage, nonExistingDBName(databaseName));
				return false;
			}
		}
		return true;	
	}
	
	
	String existingDBName(String dbName) {
		return "Nemoguce kreiranje baze sa imenom: \"" + dbName + "\", jer vec postoji baza sa tim imenom.";
	}
	String nonExistingDBName(String dbName) {
		return "Nemoguce otvaranje baze sa imenom: \"" + dbName + "\", jer ne postoji baza sa tim imenom.";
	}
	
	public void throwAlert(Stage window, String message) {
		AlertType type = AlertType.INFORMATION;
		Alert alert = new Alert(type, "");
		alert.setTitle("Greska!");
		alert.initModality(Modality.APPLICATION_MODAL);
		alert.initOwner(window);
		alert.getDialogPane().setContentText(message);
		alert.getDialogPane().setHeaderText("");
		alert.show();
		
	}
	
	
	public void setStartingButtonText(ActionEvent event) {
		if(rbCreate.isSelected()) {
			startingButton.setText("Napravi novu bazu podataka");
		}
		else if(rbOpen.isSelected()) {
			startingButton.setText("Otvori bazu podataka");
		}
	}
	
	public static String removeFileExtension(String filename) {
        int pos = filename.lastIndexOf('.');
        if (pos != -1) {
            return filename.substring(0, pos);
        }
        return filename;
    }
	public static String getExtension(String filename) {
        int pos = filename.lastIndexOf('.');
        if (pos != -1) {
        	System.out.println("ex = " + filename.substring(pos, filename.length()));
            return filename.substring(pos, filename.length());
        }
        return "mile";
    }
	

	
}
