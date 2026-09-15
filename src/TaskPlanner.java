import javax.swing.*;
import java.awt.*;
import java.util.Timer;
import java.util.TimerTask;

public class TaskPlanner extends JFrame {

    private JTextField taskField;
    private JButton addButton;
    private JButton startButton;
    private JButton stopButton;

    private DefaultListModel<String> listModel;
    private JList<String> taskList;

    private JLabel statusLabel;
    private JLabel countdownLabel;

    private Timer timer1;
    private Timer timer2;
    private Timer timer3;

    private int secondsLeft = 10;
    private int counter = 0;

    public TaskPlanner() {

        setTitle("Планировщик задач");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // Панель добавления задачи
        JPanel topPanel = new JPanel(new FlowLayout());

        taskField = new JTextField(25);
        addButton = new JButton("Добавить задачу");

        topPanel.add(new JLabel("Задача:"));
        topPanel.add(taskField);
        topPanel.add(addButton);

        // Список задач
        listModel = new DefaultListModel<>();
        taskList = new JList<>(listModel);

        JScrollPane scrollPane = new JScrollPane(taskList);

        // Кнопки управления таймерами
        JPanel buttonPanel = new JPanel();

        startButton = new JButton("Запустить таймер");
        stopButton = new JButton("Остановить");

        buttonPanel.add(startButton);
        buttonPanel.add(stopButton);

        // Информация о работе таймеров
        statusLabel = new JLabel("Статус: таймеры остановлены");
        countdownLabel = new JLabel("Обратный отсчёт: -");

        JPanel infoPanel = new JPanel(new GridLayout(2, 1));
        infoPanel.add(statusLabel);
        infoPanel.add(countdownLabel);

        // Основное расположение
        add(topPanel, BorderLayout.NORTH);
        add(scrollPane, BorderLayout.CENTER);

        JPanel bottomPanel = new JPanel(new BorderLayout());
        bottomPanel.add(buttonPanel, BorderLayout.NORTH);
        bottomPanel.add(infoPanel, BorderLayout.SOUTH);

        add(bottomPanel, BorderLayout.SOUTH);

        // Кнопка добавления задачи
        addButton.addActionListener(e -> {

            String task = taskField.getText().trim();

            if (!task.isEmpty()) {

                listModel.addElement(task);
                taskField.setText("");

                statusLabel.setText(
                        "Статус: задача добавлена"
                );

            } else {

                JOptionPane.showMessageDialog(
                        this,
                        "Введите название задачи!"
                );
            }
        });

        // Запуск таймеров
        startButton.addActionListener(e -> startTimers());

        // Остановка таймеров
        stopButton.addActionListener(e -> stopTimers());
    }

    private void startTimers() {

        // Останавливаем старые таймеры
        stopTimers();

        secondsLeft = 10;
        counter = 0;

        statusLabel.setText(
                "Статус: таймеры запущены"
        );

        /*
         * ТАЙМЕР №1
         *
         * Срабатывает один раз
         * через 5 секунд.
         */
        timer1 = new Timer();

        timer1.schedule(new TimerTask() {

            @Override
            public void run() {

                SwingUtilities.invokeLater(() -> {

                    statusLabel.setText(
                            "Статус: прошло 5 секунд"
                    );

                    JOptionPane.showMessageDialog(
                            TaskPlanner.this,
                            "Прошло 5 секунд!"
                    );
                });
            }

        }, 5000);


        /*
         * ТАЙМЕР №2
         *
         * Срабатывает каждую секунду.
         * Работает 10 секунд.
         */
        timer2 = new Timer();

        timer2.scheduleAtFixedRate(new TimerTask() {

            @Override
            public void run() {

                SwingUtilities.invokeLater(() -> {

                    if (secondsLeft >= 0) {

                        countdownLabel.setText(
                                "Обратный отсчёт: "
                                        + secondsLeft
                                        + " сек."
                        );

                        secondsLeft--;

                    } else {

                        countdownLabel.setText(
                                "Обратный отсчёт завершён"
                        );

                        this.cancel();
                    }
                });
            }

        }, 0, 1000);


        /*
         * ТАЙМЕР №3
         *
         * Срабатывает каждые 2 секунды.
         */
        timer3 = new Timer();

        timer3.scheduleAtFixedRate(new TimerTask() {

            @Override
            public void run() {

                counter++;

                SwingUtilities.invokeLater(() -> {

                    statusLabel.setText(
                            "Периодический таймер: "
                                    + counter
                                    + " срабатываний"
                    );
                });
            }

        }, 0, 2000);
    }

    private void stopTimers() {

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

        if (statusLabel != null) {
            statusLabel.setText(
                    "Статус: таймеры остановлены"
            );
        }
    }

    public static void main(String[] args) {

        SwingUtilities.invokeLater(() -> {

            TaskPlanner planner = new TaskPlanner();

            planner.setVisible(true);
        });
    }
}