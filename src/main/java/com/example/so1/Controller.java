//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package com.example.so1;

import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;

public class Controller {
    @FXML
    private TextField delayField;
    @FXML
    private TextField durationField;
    @FXML
    private TextField periodField;
    @FXML
    private TextArea logArea;
    private Timer timer3;

    private void log(String message) {
        Platform.runLater(() -> this.logArea.appendText(message + "\n"));
    }

    @FXML
    private void startTimer1() {
        int delay = Integer.parseInt(this.delayField.getText());
        this.log("Таймер 1 запущен. Сработает через " + delay + " сек...");
        final Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            {
                Objects.requireNonNull(Controller.this);
            }

            public void run() {
                Controller.this.log("Сработал!");
                timer.cancel();
            }
        }, (long)delay * 1000L);
    }

    @FXML
    private void startTimer2() {
        final int duration = Integer.parseInt(this.durationField.getText());
        this.log("Таймер 2 запущен. Будет работать " + duration + " сек (каждую секунду)...");
        final Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            int elapsed;

            {
                Objects.requireNonNull(Controller.this);
                this.elapsed = 0;
            }

            public void run() {
                ++this.elapsed;
                Controller.this.log("прошло секунд: " + this.elapsed);
                if (this.elapsed >= duration) {
                    Controller.this.log("Время истекло, стоять бояться!");
                    timer.cancel();
                }

            }
        }, 1000L, 1000L);
    }

    @FXML
    private void startTimer3() {
        if (this.timer3 != null) {
            this.timer3.cancel();
        }

        int period = Integer.parseInt(this.periodField.getText());
        this.log("Таймер 3 запущен с периодом " + period + " сек.");
        this.timer3 = new Timer();
        this.timer3.scheduleAtFixedRate(new TimerTask() {
            int count;

            {
                Objects.requireNonNull(Controller.this);
                this.count = 0;
            }

            public void run() {
                ++this.count;
                Controller.this.log("Срабатывание №" + this.count);
            }
        }, 0L, (long)period * 1000L);
    }

    @FXML
    private void stopTimer3() {
        if (this.timer3 != null) {
            this.timer3.cancel();
            this.timer3 = null;
            this.log("Остановитесь!");
        }

    }
}
