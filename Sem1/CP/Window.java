import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class Window extends JFrame
{
  private Jpanel north = new JPanel();
  private Jpanel  south = new JPanel();
  private Jpanel  center = new JPanel();
  private Jpanel  east = new JPanel();
  private Jpanel  west = new JPanel();
  
  JLabel label1 = new JLabel ("Ичич зарар");
  JLabel label2 = new JLabel ("Никакие претензии к софту не принимаются");
  JLabel label3 = new JLabel ("Путь к файлу:");
  JTextField field = new JTextField (30);
  JButton button = new JButton ("Click here");

  JMenuItem openItem = new JMenuItem ("Открыть файл");
  JMenuItem openItem2 = new JMenuItem ("Сохранить резултат");
  JMenuItem openItem3 = new JMenuItem ("Выход");

  public Window()
  {
    super ("Name");
    Toolkit theKit = this.getToolkit();
    Dimension screen = theKit.getScreenSize();
    setBounds (screen.width/4, screen.height/4, screen.width/2, screen.height/2);
    setDefaultCloseOperation(JFreme.EXIT_ON_CLOSE);

    north.setBackground(new color(220, 220, 255));
    south.setBackground(new color(255, 200, 220));
    center.setBackground(new color(200, 255, 255));
    east.setBackground(new color(255, 200, 200));
    west.setBackground(new color(200, 200, 255));
  
    east.add(label1);
    south.add(label2);
    north.add(label3);
    north.add(field);
    north.add(button);

    this.add(north, BorderLayout.NORTH);
    this.add(south, BorderLayout.SOUTH);
    this.add(center, BorderLayout.CENTER);
    this.add(east, BorderLayout.EAST);
    this.add(west, BorderLayout.WEST);

    this.setVisible(true);
  }
}
