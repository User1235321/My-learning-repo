package swingtest;

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


public class TestWindow extends JFrame implements ActionListener {
    private JLabel label1 = new JLabel("<html>Просто<br/>текст</html>");
    private JPanel north = new JPanel();
    private JPanel east = new JPanel();
    private JPanel south = new JPanel();
    private JPanel west = new JPanel();
    private JPanel center = new JPanel();
    private JScrollPane jsp = new JScrollPane(center);
    private JLabel label2 = new JLabel("Текстовое поле:");
    private JTextField tf1 = new JTextField(50);
    private JTextArea ta1 = new JTextArea(20,30);
    private JMenuBar mb = new JMenuBar();
    private JMenu fileMenu = new JMenu("File");
    private JMenuItem openItem = new JMenuItem("Open input file");
    private JMenuItem saveItem = new JMenuItem("Save result file");
    private JMenuItem quitItem = new JMenuItem("Quit");
    private JButton processButton = new JButton("Process");
    private JButton saveButton = new JButton("Save result");
    private JButton quitButton = new JButton("Quit");
    private JButton chooseFileButton = new JButton("Choose File");
    private JLabel pathLabel = new JLabel(); 
    
     
    public TestWindow(String title) {
        super(title);
        Toolkit theKit = this.getToolkit();
        Dimension winSize = theKit.getScreenSize();
        this.setBounds(winSize.width/4,winSize.height/4,winSize.width/2,winSize.height/2);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        this.setJMenuBar(mb);
        mb.add(fileMenu);
        fileMenu.add(openItem);
        fileMenu.add(saveItem);
        fileMenu.add(quitItem);
        
        // раскладываем панели
        north.setBackground(Color.WHITE);
        south.setBackground(new Color(200,200,255));
        east.setBackground(new Color(255,255,200));
        west.setBackground(new Color(255,200,200));
        center.setBackground(new Color(200,255,200));
        //
        east.add(label1);
        south.add(label2); south.add(tf1);
        center.setLayout(new BorderLayout());
        center.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.green, 3),
                BorderFactory.createEmptyBorder(10, 10, 10, 10)));
      
        center.add(ta1,BorderLayout.CENTER);
        west.setLayout(new GridLayout(12,1));
        west.add(processButton); west.add(saveButton); west.add(quitButton);
        
        north.setLayout(new BorderLayout());
        north.add(pathLabel,BorderLayout.CENTER);
        north.add(chooseFileButton,BorderLayout.EAST);
        chooseFileButton.addActionListener(this);
        this.add(north, BorderLayout.NORTH);
        this.add(south, BorderLayout.SOUTH);
        this.add(east, BorderLayout.EAST);
        this.add(west, BorderLayout.WEST);
        this.add(center, BorderLayout.CENTER);
        
        this.setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        JFileChooser fch = new JFileChooser();
        switch(fch.showOpenDialog(this)) {
            case JFileChooser.APPROVE_OPTION:
                try( BufferedReader br = new BufferedReader(
                        new FileReader(fch.getSelectedFile()))) {
                    String s;
                    while( (s = br.readLine()) != null) {
                        ta1.setText(ta1.getText() + "\n" + s);
                    }
                
                }   catch (IOException ex) {
                    Logger.getLogger(TestWindow.class.getName()).log(Level.SEVERE, null, ex);
                }
                break;
            default:
                ;
        }
    }

}
