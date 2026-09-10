package volodea;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Calendar;
import java.util.Date;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

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
        GuiTimerVova guiTimerVova = new GuiTimerVova();
        guiTimerVova.startApp();
        consoleTimerVane.runwithDelay(3000);
        consoleTimerVane.runAtDateTimw(5000);
        consoleTimerVane.runFixedDelay(1000,2000);
        consoleTimerVane.runFixedRate(1000,2000);
    }
}

class GuiTimerVova{
    int currProgress = 0;

    JFrame frame = new JFrame("First lab - GUI interface");

    JProgressBar progressBar = new JProgressBar(0, 100);
    JLabel progressLabel = new JLabel("Статус: Ожидание");
    JLabel title = new JLabel("Steam Installer: Горячий Мучачос");
    JButton startButton = new JButton("Start");
    JButton pauseButton = new JButton("Pause");

    ActionListener actionListener = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            currProgress += 1;
            progressBar.setValue(currProgress);

            if(currProgress >= 100){
                ((Timer) e.getSource()).stop();
                progressLabel.setText("Статус: Завершён");
                startButton.setEnabled(true);
            }
        }
    };

    Timer timer = new Timer(100, actionListener);

    ActionListener actionListenerStarter = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            progressLabel.setText("Статус: Установка...");
            startButton.setEnabled(false);
            timer.start();
        }
    };

    ActionListener actionListenerPause = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            progressLabel.setText("Статус: Ожидание...");
            startButton.setEnabled(true);
            timer.stop();
        }
    };

    GuiTimerVova() {
        frame.setSize(400, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));
    }

    private void setComponentsOnWindow() {
        frame.add(title);
        frame.add(progressBar);
        frame.add(progressLabel);
        frame.add(startButton);
        frame.add(pauseButton);
    }

    public void startApp() {
        setComponentsOnWindow();

        startButton.addActionListener(actionListenerStarter);
        pauseButton.addActionListener(actionListenerPause);

        frame.setVisible(true);
    }
}