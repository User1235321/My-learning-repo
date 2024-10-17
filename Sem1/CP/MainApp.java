import javax.swing.JFrame;

public class MainApp {

public static void main(String[] args) {

// Создаем экземпляр JFrame (окно)

JFrame frame = new JFrame("Мое первое оконное приложение");

// Устанавливаем размер окна

frame.setSize(400, 300);

// Завершаем работу, когда пользователь закроет окно

frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

// Делаем окно видимым

frame.setVisible(true);

}

}
