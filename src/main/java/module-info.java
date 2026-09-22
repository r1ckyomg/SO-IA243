module com.example.so1 {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.so1 to javafx.fxml;
    exports com.example.so1;
}