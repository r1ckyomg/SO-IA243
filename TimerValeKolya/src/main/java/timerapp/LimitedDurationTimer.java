package timerapp;

import java.util.Timer;
import java.util.TimerTask;

public class LimitedDurationTimer {

    public void start(long duration) {

        Timer timer = new Timer();

        TimerTask task = new TimerTask() {

            private int seconds = 0;

            @Override
            public void run() {

                seconds++;

                System.out.println(
                        "Таймер работает: "
                                + seconds
                                + " сек."
                );

                if (seconds >= duration / 1000) {
                    System.out.println("Время работы таймера закончилось");
                    timer.cancel();
                }
            }
        };

        timer.scheduleAtFixedRate(task, 0, 1000);
    }
}