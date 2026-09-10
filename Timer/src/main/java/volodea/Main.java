package volodea;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Calendar;
import java.util.Date;

class consoleTimerVane {
    static void runwithDelay(long delay) {
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                System.out.println("Таймер работает");
                System.out.println("прошло " + delay / 1000 + " секунд");
                timer.cancel();
            }
        }, delay);
    }

    static void runAtDateTimw(int delay) {
        Timer timer = new Timer();
        Calendar calendar = Calendar.getInstance();
        calendar.add(Calendar.SECOND, delay / 1000);
        Date time = calendar.getTime();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                System.out.println("Будильник сработал!");

                timer.cancel();
            }

        }, time);
    }

    static void runFixedDelay(long delay, long period) {
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            int counter = 0;

            @Override
            public void run() {
                counter++;
                System.out.println("Запуск # " + counter + " прошла " + counter + " секунд");
                if (counter >= 5) {
                    cancel();
                    timer.cancel();
                    System.out.println("Таймер остановлен");
                }
            }

        }, delay, period);
    }

    static void runFixedRate(long delay, long period) {
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            int counter = 0;
            @Override
            public void run() {
                counter++;
                System.out.println(
                        "Запуск # " + counter  + "столкьо секунд прошло"
                );
                if (counter >= 5) {
                    cancel();
                    timer.cancel();

                    System.out.println("Таймер остановлен");
                }
            }

        }, delay, period);
    }
}
public class Main {
    static void main() {
        consoleTimerVane.runwithDelay(3000);
        consoleTimerVane.runAtDateTimw(5000);
        consoleTimerVane.runFixedDelay(1000,2000);
        consoleTimerVane.runFixedRate(1000,2000);
    }
}

class GuiTimerVova{

}