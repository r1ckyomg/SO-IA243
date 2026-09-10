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
    JFrame frame = new JFrame("First lab - GUI interface");

    JProgressBar progressBar = new JProgressBar(0, 100);
    JLabel progressLabel = new JLabel("Статус: Ожидание");
    JButton startButton = new JButton("Start");

    ActionListener actionListener = new ActionListener() {
        int currProgress = 0;

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
            progressLabel.setText("Статус: Выполнение...");
            startButton.setEnabled(false);
            timer.start();
        }
    };

    GuiTimerVova() {
        frame.setSize(400, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));
    }

    private void setComponentsOnWindow() {
        frame.add(progressBar);
        frame.add(progressLabel);
        frame.add(startButton);
    }

    public void startApp() {
        setComponentsOnWindow();

        startButton.addActionListener(actionListenerStarter);

        frame.setVisible(true);
    }
}