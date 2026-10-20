
package labtimer;

import javax.swing.*;
import java.awt.*;
import java.util.Timer;
import java.util.TimerTask;

public class TimerPlanner extends JFrame {

    private final JTextArea log = new JTextArea();
    private Timer timer;
    private int remainingSeconds;

    public TimerPlanner() {
        setTitle("Планировщик процессов");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        log.setEditable(false);

        JButton once = new JButton("Однократный запуск");
        JButton countdown = new JButton("Обратный отсчёт");
        JButton periodic = new JButton("Периодический запуск");
        JButton stop = new JButton("Остановить");

        JPanel panel = new JPanel(new GridLayout(2, 2, 5, 5));
        panel.add(once);
        panel.add(countdown);
        panel.add(periodic);
        panel.add(stop);

        add(panel, BorderLayout.NORTH);
        add(new JScrollPane(log), BorderLayout.CENTER);

        once.addActionListener(e -> startOnce());
        countdown.addActionListener(e -> startCountdown());
        periodic.addActionListener(e -> startPeriodic());
        stop.addActionListener(e -> stopTimer());
    }

    private void startOnce() {
        stopTimer();
        timer = new Timer();

        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                writeLog("Однократная задача выполнена.");
                timer.cancel();
            }
        }, 3000);

        writeLog("Задача запланирована через 3 секунды.");
    }

    private void startCountdown() {
        stopTimer();
        remainingSeconds = 10;
        timer = new Timer();

        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                if (remainingSeconds <= 0) {
                    writeLog("Обратный отсчёт завершён.");
                    timer.cancel();
                    return;
                }

                writeLog("Осталось секунд: " + remainingSeconds);
                remainingSeconds--;
            }
        }, 0, 1000);
    }

    private void startPeriodic() {
        stopTimer();
        timer = new Timer();

        timer.scheduleAtFixedRate(new TimerTask() {
            private int count = 1;

            @Override
            public void run() {
                writeLog("Выполнение задачи №" + count++);
            }
        }, 0, 2000);
    }

    private void stopTimer() {
        if (timer != null) {
            timer.cancel();
            timer = null;
            writeLog("Таймер остановлен.");
        }
    }

    private void writeLog(String message) {
        SwingUtilities.invokeLater(() -> log.append(message + "\n"));
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new TimerPlanner().setVisible(true);
        });
    }
}
