package timerapp;

import java.util.Timer;
import java.util.TimerTask;

public class DelayedTimer {

    public void start(long delay) {

        Timer timer = new Timer();

        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                System.out.println(
                        "Таймер сработал через "
                                + delay / 1000
                                + " секунд"
                );

                timer.cancel();
            }
        };

        timer.schedule(task, delay);
    }
}