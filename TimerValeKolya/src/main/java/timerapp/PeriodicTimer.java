package timerapp;

import java.util.Timer;
import java.util.TimerTask;

public class PeriodicTimer {

    private Timer timer;
    private int counter = 0;

    public void start(long period) {

        timer = new Timer();

        TimerTask task = new TimerTask() {
            @Override
            public void run() {

                counter++;

                System.out.println(
                        "Периодический таймер сработал. Количество: "
                                + counter
                );
            }
        };

        timer.scheduleAtFixedRate(task, 0, period);
    }

    public void stop() {

        if (timer != null) {
            timer.cancel();
            timer = null;
        }

        System.out.println("Периодический таймер остановлен");
    }
}
