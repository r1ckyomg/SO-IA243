package timerapp;

import javax.swing.*;
import java.awt.*;

public class MainWindow extends JFrame {

    private final DelayedTimer delayedTimer = new DelayedTimer();
    private final LimitedDurationTimer limitedTimer = new LimitedDurationTimer();
    private final PeriodicTimer periodicTimer = new PeriodicTimer();

    public MainWindow() {

        setTitle("Timer Manager");
        setSize(500, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        setLayout(new GridLayout(3, 1, 10, 10));

        // ---------- Таймер с задержкой ----------
        JPanel delayedPanel = new JPanel();

        JLabel delayedLabel = new JLabel("Задержка (сек):");
        JTextField delayedField = new JTextField("5", 5);
        JButton delayedButton = new JButton("Запустить");

        delayedPanel.add(delayedLabel);
        delayedPanel.add(delayedField);
        delayedPanel.add(delayedButton);

        delayedButton.addActionListener(e -> {
            try {
                long seconds = Long.parseLong(delayedField.getText());

                if (seconds <= 0) {
                    JOptionPane.showMessageDialog(
                            this,
                            "Введите число больше 0"
                    );
                    return;
                }

                delayedTimer.start(seconds * 1000);

                JOptionPane.showMessageDialog(
                        this,
                        "Таймер запущен на " + seconds + " сек."
                );

            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(
                        this,
                        "Введите корректное число"
                );
            }
        });

        // ---------- Таймер ограниченного времени ----------
        JPanel limitedPanel = new JPanel();

        JLabel limitedLabel = new JLabel("Время работы (сек):");
        JTextField limitedField = new JTextField("10", 5);
        JButton limitedButton = new JButton("Запустить");

        limitedPanel.add(limitedLabel);
        limitedPanel.add(limitedField);
        limitedPanel.add(limitedButton);

        limitedButton.addActionListener(e -> {
            try {
                long seconds = Long.parseLong(limitedField.getText());

                if (seconds <= 0) {
                    JOptionPane.showMessageDialog(
                            this,
                            "Введите число больше 0"
                    );
                    return;
                }

                limitedTimer.start(seconds * 1000);

            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(
                        this,
                        "Введите корректное число"
                );
            }
        });

        // ---------- Периодический таймер ----------
        JPanel periodicPanel = new JPanel();

        JLabel periodicLabel = new JLabel("Период (сек):");
        JTextField periodicField = new JTextField("2", 5);
        JButton startPeriodicButton = new JButton("Запустить");
        JButton stopPeriodicButton = new JButton("Остановить");

        periodicPanel.add(periodicLabel);
        periodicPanel.add(periodicField);
        periodicPanel.add(startPeriodicButton);
        periodicPanel.add(stopPeriodicButton);

        startPeriodicButton.addActionListener(e -> {
            try {
                long seconds = Long.parseLong(periodicField.getText());

                if (seconds <= 0) {
                    JOptionPane.showMessageDialog(
                            this,
                            "Введите число больше 0"
                    );
                    return;
                }

                periodicTimer.start(seconds * 1000);

            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(
                        this,
                        "Введите корректное число"
                );
            }
        });

        stopPeriodicButton.addActionListener(e -> {
            periodicTimer.stop();
        });

        add(delayedPanel);
        add(limitedPanel);
        add(periodicPanel);
    }
}