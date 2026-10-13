
import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Timer;
import java.util.TimerTask;

public class Main extends JFrame {

    private final JTextArea logArea = new JTextArea(13, 48);

    private final JTextField delayField = new JTextField("5", 5);
    private final JTextField durationField = new JTextField("10", 5);
    private final JTextField periodField = new JTextField("2", 5);

    private Timer timer1;
    private Timer timer2;
    private Timer timer3;

    public Main() {
        setTitle("Лабораторная работа №1 - Таймеры");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout(10, 10));

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(4, 1, 5, 5));

        JPanel row1 = new JPanel(new FlowLayout(FlowLayout.LEFT));
        row1.add(new JLabel("Таймер 1. Задержка (сек):"));
        row1.add(delayField);

        JButton start1 = new JButton("Запустить");
        start1.addActionListener(e -> startTimer1());
        row1.add(start1);
        panel.add(row1);

        JPanel row2 = new JPanel(new FlowLayout(FlowLayout.LEFT));
        row2.add(new JLabel("Таймер 2. Длительность (сек):"));
        row2.add(durationField);

        JButton start2 = new JButton("Запустить");
        start2.addActionListener(e -> startTimer2());
        row2.add(start2);
        panel.add(row2);

        JPanel row3 = new JPanel(new FlowLayout(FlowLayout.LEFT));
        row3.add(new JLabel("Таймер 3. Период (сек):"));
        row3.add(periodField);

        JButton start3 = new JButton("Запустить");
        start3.addActionListener(e -> startTimer3());

        JButton stop3 = new JButton("Стоп");
        stop3.addActionListener(e -> stopTimer3());

        row3.add(start3);
        row3.add(stop3);
        panel.add(row3);

        JPanel row4 = new JPanel();
        JButton stopAll = new JButton("Остановить все таймеры");
        stopAll.addActionListener(e -> stopAllTimers());
        row4.add(stopAll);
        panel.add(row4);

        add(panel, BorderLayout.NORTH);

        logArea.setEditable(false);
        logArea.setFont(new Font("Monospaced", Font.PLAIN, 13));
        add(new JScrollPane(logArea), BorderLayout.CENTER);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                cancelTimers();
            }
        });

        pack();
        setLocationRelativeTo(null);
        setVisible(true);

        log("Программа запущена");
    }

    private void log(String message) {
        SwingUtilities.invokeLater(() -> {
            logArea.append(message + "\n");
            logArea.setCaretPosition(logArea.getDocument().getLength());
        });
    }

    private int readSeconds(JTextField field) {
        try {
            int value = Integer.parseInt(field.getText().trim());

            if (value < 1 || value > 3600) {
                throw new NumberFormatException();
            }

            return value;

        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(
                this,
                "Введите целое число от 1 до 3600",
                "Ошибка ввода",
                JOptionPane.ERROR_MESSAGE
            );
            return -1;
        }
    }

    private void startTimer1() {
        int seconds = readSeconds(delayField);
        if (seconds == -1) return;

        if (timer1 != null) timer1.cancel();

        timer1 = new Timer("Timer-1", true);

        log("Таймер 1: ожидание " + seconds + " сек.");

        timer1.schedule(new TimerTask() {
            @Override
            public void run() {
                log("Таймер 1: время истекло!");
                Toolkit.getDefaultToolkit().beep();
            }
        }, seconds * 1000L);
    }

    private void startTimer2() {
        int seconds = readSeconds(durationField);
        if (seconds == -1) return;

        if (timer2 != null) timer2.cancel();

        Timer currentTimer = new Timer("Timer-2", true);
        timer2 = currentTimer;

        log("Таймер 2: работает " + seconds + " сек.");

        currentTimer.scheduleAtFixedRate(new TimerTask() {
            int count = 0;

            @Override
            public void run() {
                count++;

                // Не выводим лишние сообщения после остановки
                if (timer2 != currentTimer) return;

                log("Таймер 2: прошла секунда " + count);

                if (count >= seconds) {
                    log("Таймер 2: работа завершена");
                    currentTimer.cancel();

                    if (timer2 == currentTimer) {
                        timer2 = null;
                    }
                }
            }
        }, 1000L, 1000L);
    }

    private void startTimer3() {
        int seconds = readSeconds(periodField);
        if (seconds == -1) return;

        if (timer3 != null) timer3.cancel();

        timer3 = new Timer("Timer-3", true);

        log("Таймер 3: период " + seconds + " сек.");

        timer3.scheduleAtFixedRate(new TimerTask() {
            int count = 0;

            @Override
            public void run() {
                count++;
                log("Таймер 3: сигнал №" + count); // comment
                Toolkit.getDefaultToolkit().beep();
            }
        }, 0L, seconds * 1000L);
    }

    private void stopTimer3() {
        if (timer3 != null) {
            timer3.cancel();
            timer3 = null;
            log("Таймер 3 остановлен");
        }
    }

    private void cancelTimers() {
        if (timer1 != null) {
            timer1.cancel();
            timer1 = null;
        }

        if (timer2 != null) {
            timer2.cancel();
            timer2 = null;
        }

        if (timer3 != null) {
            timer3.cancel();
            timer3 = null;
        }
    }

    private void stopAllTimers() {
        cancelTimers();
        log("Все таймеры остановлены");
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Main::new);
    }
}
