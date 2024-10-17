import java.io.*;
import java.util.ArrayList;

public class p2 {
    public static void main(String[] args) {
//        int b[] = {-2,5,-7,6,3,1,-15,10,12,-1,5,-7,1};
//        for (int i = 0; i < b.length ; i++) {
//            System.out.print(" b["+i+"]="+b[i]);
//        }
        ArrayList<String> stroki = new ArrayList<>();
        try(BufferedReader bf = new BufferedReader(new InputStreamReader(
                new FileInputStream("lineqs.csv"),"UTF8"))) {
                String s;
                while(( s = bf.readLine()) != null) {
                    stroki.add(s);
                    System.out.println(s);
                }
                System.out.println("\nRead from file: " + stroki.size() + " lines");
        } catch (FileNotFoundException e) {
            System.out.println("Файл не найден");
        } catch (IOException e) {
            System.out.println("Ошибка чтения");
        }
        int n = stroki.size(); // 9 уравнений
        double a[][] = new double[n][n+1];
        for (int i = 0; i < n; i++) {
            String strNums[] = stroki.get(i).split(",");
            for (int j = 0; j <= n; j++) {
                a[i][j] = Double.parseDouble(strNums[j]);
            }
        }
        // проверим матрицу а:
        
       for(int i = 0; i < (n-1); ++i){
       	for(int j = i; j <= n; ++j){
        if(a[i][j] > a[i+1][j]){
          for(int q = 0; q <= n;++q){
           a[i][q] += a[i+1][q];
           a[i+1][q] = a[i][q] - a[i+1][q];
           a[i][q] -= a[i+1][q];
          }
        }
       	}
       }
       
       double dop = 0;
       
       for (int q = 0; q < n; ++q)
       {
		for(int i = (q+1); i < n; ++i)
		{
			dop = (a[i][q]/a[q][q]);
		        for(int j = q; j <= n; ++j)
			{
				a[i][j] -= (a[q][j] * dop);
			}
		}
        
        for(int i = (q+1); i < (n-1); ++i){
        	if(Math.abs(a[q+1][i]) < Math.abs(a[i+1][i])){
          		for(int b = q; b <= n; ++b){
          			a[q+1][b] += a[i+1][b];
           			a[i+1][b] = a[q+1][b] - a[i+1][b];
          			a[q+1][b] -= a[i+1][b];
         		 }
        	}
       }
       
       double arr[n];
       for(int i = n-1; i >=0; --i){
       	for(int j = ){
		a[i][i+1]
       	}
       	       	arr[i] = a[i][i+1]/a[i][i];
       }
       
       }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j <= n; j++) {
                System.out.print(a[i][j] + " ");
            }
            System.out.println();
        }

    }
}
