package volodea;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

class consoleTimerVane {

}


public class Main {
    static void main() {
        GuiTimerVova guiTimerVova = new GuiTimerVova();
        guiTimerVova.startApp();
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