import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class TimerLab extends JFrame {
    private final JLabel delayedLabel = new JLabel("Действие через 3 секунды: ожидание");
    private final JLabel periodicLabel = new JLabel("Секунд прошло: 0 из 15");
    private final JButton startButton = new JButton("Старт");
    private final JButton stopButton = new JButton("Стоп");
    
    private Timer delayedTimer;
    private javax.swing.Timer swingTimer;
    private int seconds;
    private long generation;

    public TimerLab() {
        super("Общий Вариант Таймеров");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel controls = new JPanel(new FlowLayout());
        controls.add(startButton);
        controls.add(stopButton);

        JPanel status = new JPanel();
        status.setLayout(new javax.swing.BoxLayout(status, javax.swing.BoxLayout.Y_AXIS));
        status.add(delayedLabel);
        status.add(periodicLabel);

        add(status, BorderLayout.CENTER);
        add(controls, BorderLayout.SOUTH);
        startButton.addActionListener(e -> startTimers());
        stopButton.addActionListener(e -> stopTimers());
        stopButton.setEnabled(false);

        setSize(390, 150);
        setLocationRelativeTo(null);
    }

    private void startTimers() {
        stopTimers();
        seconds = 0;
        delayedLabel.setText("Действие через 3 секунды: ожидание");
        periodicLabel.setText("Секунд прошло: 0 из 15");
        startButton.setEnabled(false);
        stopButton.setEnabled(true);
        final long currentGeneration = generation;

        delayedTimer = new Timer("delayed-timer", true);
        delayedTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                SwingUtilities.invokeLater(() -> {
                    if (currentGeneration == generation) {
                        delayedLabel.setText("Действие через 3 секунды: выполнено");
                    }
                });
            }
        }, 3000);

        swingTimer = new javax.swing.Timer(1000, e -> {
            seconds++;
            periodicLabel.setText("Секунд прошло: " + seconds + " из 15");
            if (seconds == 15) {
                stopTimers();
                periodicLabel.setText("Готово: 15 действий за 15 секунд");
            }
        });
        swingTimer.start();
    }

    private void stopTimers() {
        generation++;
        if (delayedTimer != null) {
            delayedTimer.cancel();
            delayedTimer = null;
        }
        if (swingTimer != null) {
            swingTimer.stop();
            swingTimer = null;
        }
        startButton.setEnabled(true);
        stopButton.setEnabled(false);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new TimerLab().setVisible(true));
    }
}
