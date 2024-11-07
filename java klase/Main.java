package application;
	
import java.io.IOException;
import java.util.Optional;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ButtonType;
import javafx.scene.image.Image;


public class Main extends Application {
	
	static {
        System.loadLibrary("bibliotekaZaBazu");
    }

	
	public static Database db = new Database("");
	
	
	
	@Override
	public void start(Stage primaryStage) {
		try {
			initPrimaryStage(primaryStage);
			
			Scene scene = loadBeginScene();
			scene.setUserData("BeginScena");
			scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
			primaryStage.setScene(scene);
		
			
			primaryStage.setOnCloseRequest(e -> {
				e.consume();
				closeWindowSafe(primaryStage);
			}); 
			
			primaryStage.show();

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}
	
	private void initPrimaryStage(Stage primStage) {
		primStage.setTitle("BPApp");
		Image icon = new Image(getClass().getResourceAsStream("icon.png"));
		primStage.getIcons().add(icon);
	}
	
	private Scene loadBeginScene() throws IOException {
		Parent root  = FXMLLoader.load(getClass().getResource("Begin.fxml"));			
		return new Scene(root);
	}
	
	public void closeWindowSafe(Stage window) {
		ButtonType yesButton = new ButtonType("Da");
        ButtonType noButton = new ButtonType("Ne");
		
		AlertType type = AlertType.CONFIRMATION;
		Alert alert = new Alert(type, "");
		alert.setTitle("Potvrda");
		alert.initModality(Modality.APPLICATION_MODAL);
		alert.initOwner(window);
		alert.getDialogPane().setContentText("Da li ste sigurni da zelite da napustite aplikaciju?");
		alert.getDialogPane().setHeaderText("");
		alert.getButtonTypes().setAll(yesButton, noButton);
		Optional <ButtonType> result = alert.showAndWait();
		if(result.get() == yesButton) {
			if(window.getScene().getUserData().equals("MainScena") && !db.getSaved() ) saveBeforeClosing(window);
			window.close();	
		}
		
	}
	public void saveBeforeClosing(Stage window) {
		ButtonType yesButton = new ButtonType("Da");
        ButtonType noButton = new ButtonType("Ne");
		
		AlertType type = AlertType.CONFIRMATION;
		Alert alert = new Alert(type, "");
		alert.setTitle("Potvrda");
		alert.initModality(Modality.APPLICATION_MODAL);
		alert.initOwner(window);
		alert.getDialogPane().setContentText("Da li zelite da sacuvate bazu pre zatvaranja?");
		alert.getDialogPane().setHeaderText("");
		alert.getButtonTypes().setAll(yesButton, noButton);
		Optional <ButtonType> result = alert.showAndWait();
		if(result.get() == yesButton) {
			db.saveDB(false);	
				
		}
	}
	
}
