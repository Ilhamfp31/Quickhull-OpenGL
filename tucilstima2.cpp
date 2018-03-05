// Nama: Ilham Firdausi Putra
// NIM: 13516140

// Kompilasi: g++ tucilstima2.cpp -lopengl32 -lglew32 -lglfw3dll

#include <bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glfw3.h>

using namespace std;

vector< pair<int,int> > vectorPoint;
vector<pair<int, int>> titikHasil;
pair<int, int> titikTengah;
 

int cariSisi(pair<int, int> p1, pair<int, int> p2, pair<int, int> p)
{
  int cek = ((p1.first - p.first) * (p2.second - p.second)) - ((p2.first - p.first) * (p1.second - p.second));

  if (cek > 0){
    return 1;// kiri
  } else if (cek < 0){
    return -1; // kanan
  } else {
    return 0; // segaris
  }
    
}

void selesaikanConvexHull(vector< pair<int,int> > a,  pair<int, int> p1, pair<int, int> p2, int sisi)
{
  int indeks = -1;
  int jarakMaks = 0;

  for (int i=0; i<a.size(); i++)
  {
    // Cari jarak garis ke titik
    int temp = (abs ( ((p2.second - p1.second)*a[i].first) - ((p2.first - p1.first)*a[i].second) + (p2.first * p1.second) - (p2.second * p1.first) )) / sqrt( pow( (p2.second - p1.second),2 ) + pow( (p2.first - p1.first),2 ) );
    if ((cariSisi(p1, p2, a[i]) == sisi) && (temp > jarakMaks) )
    {
      indeks = i;
      jarakMaks = temp;
    }
  }

  // Tidak ketemu titik berarsi sudah ketemu basis.
  if (indeks == -1)
  {
    pair< int,int > titik1 = make_pair(p1.first , p1.second);
    pair< int,int > titik2 = make_pair(p2.first , p2.second);

    auto it1 = find(titikHasil.begin(), titikHasil.end(), titik1);
    if (it1 == titikHasil.end())
    {
      titikHasil.push_back( titik1 );
    }

    auto it2 = find(titikHasil.begin(), titikHasil.end(), titik2);
    if (it2 == titikHasil.end())
    {
      titikHasil.push_back( titik2 );
    }
  } else {
    //printf("Titik =\n p1=(%d,%d) \n p2=(%d,%d) \n p=(%d,%d) \n Sisi sekarang = %d\n sisi cari1 = %d\n sisi cari2 = %d\n", p1.first, p1.second, p2.first, p2.second, a[indeks].first, a[indeks].second, sisi, -cariSisi(a[indeks], p1, p2), -cariSisi(a[indeks], p2, p1)  );
    selesaikanConvexHull(a, p1, a[indeks], cariSisi(a[indeks], p1, p2));
    selesaikanConvexHull(a, p2, a[indeks], cariSisi(a[indeks], p2, p1));
  }
    
}
 

// Sort titik-titik searah jarum jam
bool cmpSort(pair<int, int> a, pair<int, int> b)
{
  //Pertama cek berdasarkan letaknya
  if (a.first - titikTengah.first >= 0 && b.first - titikTengah.first < 0){
    return true;
  }
  if (a.first - titikTengah.first < 0 && b.first - titikTengah.first >= 0){
    return false;
  }

  if (a.first - titikTengah.first == 0 && b.first - titikTengah.first == 0) {
    if (a.second - titikTengah.second >= 0 || b.second - titikTengah.second >= 0){
      return a.second > b.second;
    }
    return b.second > a.second;
  }

  // cek cross product
  int det = (a.first - titikTengah.first) * (b.second - titikTengah.second) - (b.first - titikTengah.first) * (a.second - titikTengah.second);
  if (det < 0){
    return true;
  }
  if (det > 0){
    return false;
  }

  //Dalam kasus ekstrim, jika ke-3 titik berada dalam 1 titik cek mana yang lebih jauh 
  int d1 = (a.first - titikTengah.first) * (a.first - titikTengah.first) + (a.second - titikTengah.second) * (a.second - titikTengah.second);
  int d2 = (b.first - titikTengah.first) * (b.first - titikTengah.first) + (b.second - titikTengah.second) * (b.second - titikTengah.second);

  return d1 > d2;
}

// Gambar convex hull
void drawGaris()
{
  glColor3f(0.0, 0.0, 1.0);

  titikHasil.push_back(titikHasil[0]);
  for (int i = 0; i < (int) titikHasil.size()-1; ++i)
  {
    // Set besar garis
    glLineWidth(3.0);

    glBegin(GL_LINES);
      int x1 = titikHasil[i].first;
      int y1 = titikHasil[i].second;
      int x2 = titikHasil[i+1].first;
      int y2 = titikHasil[i+1].second;
      glVertex2f ( (float) x1 / (float) 110 , (float) y1 / (float) 110 );
      glVertex2f ( (float) x2 / (float) 110 , (float) y2 / (float) 110 );
    glEnd();

  }

  glFlush();

}

// Gambar garis kartesian
void drawCartesian()
{
  // Warna putih
  glColor3f(1.0, 1.0, 1.0);

  // Set besar garis
  glLineWidth(3.0);

  // Gambar garis
  glBegin(GL_LINES);
    glVertex2f (-1.0, 0.0);
    glVertex2f (1.0, 0.0);
  glEnd();
  glFlush();  

  glBegin(GL_LINES);
    glVertex2f (0.0, -1.0);
    glVertex2f (0.0, 1.0);
  glEnd();
  glFlush();  
}

void drawTitik()
{
  // Warna
  glColor3f(1.0, 0.0, 0.0);

  // Gambar titik
  for (int i = 0; i < (int) vectorPoint.size(); ++i)
  {
    // Set besar titik
    glPointSize(4.0);

    glBegin(GL_POINTS);
      int x = vectorPoint[i].first;
      int y = vectorPoint[i].second;	
      glVertex2f ( (float) x / (float) 110 , (float) y / (float) 110 );
    glEnd();

  }
  glFlush();	
  }

int main()
{

  // Initialise GLFW
  if (glfwInit())
  {
  // Membuat window 600x600 
  GLFWwindow* window = glfwCreateWindow(600, 600, "Tugas Kecil 2 Strategi Algoritma", NULL, NULL);
  glewExperimental = true;
  glewInit();
  glfwMakeContextCurrent(window);

  string perintah;

  do{

      titikHasil.clear();
      vectorPoint.clear();

      //Gambar warna background dan peta kartesian kosong
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
      drawCartesian();
      glfwSwapBuffers(window);
      glfwPollEvents();

      int banyakTitik;
      cout<<"Masukan banyak titik = ";
      cin>>banyakTitik;

      for (int i = 0; i < banyakTitik; ++i)
      {
        bool ketemu = false;
        while(!ketemu){
          int xTemp = (rand() % 201) - 100;
          int yTemp = (rand() % 201) - 100;
          pair<int,int> tempTitik(xTemp,yTemp);
          auto it = find(vectorPoint.begin(), vectorPoint.end(), tempTitik);
          if (it == vectorPoint.end())
          {
            ketemu = true;
            vectorPoint.push_back(tempTitik);
            printf("Hasil random = (%d,%d)\n", xTemp, yTemp);
          } 
        }

      }

      if (banyakTitik == 1)
      {
        clock_t t1,t2;
        t1=clock();
        t2=clock();
        float diff ((float)t2-(float)t1);
        float seconds = diff / CLOCKS_PER_SEC;
        printf("Lama mengkomputasi hasil (di luar input&output) = %f detik \n", seconds);
        printf("Spek Komputer: Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz, 2400 Mhz, 2 Core(s), 4 Logical Processor(s), 12.0 GB Installed Physical Memory (RAM) \n");


        printf("Hasil convex hull =\n");
        cout << "(" << vectorPoint[0].first << ", " << vectorPoint[0].second << ") ";
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        drawCartesian();
        drawTitik();
        glfwSwapBuffers(window);
        glfwPollEvents();

        } else if (banyakTitik == 2){
        clock_t t1,t2;
        t1=clock();

        int x1 = vectorPoint[0].first;
        int y1 = vectorPoint[0].second;
        int x2 = vectorPoint[1].first;
        int y2 = vectorPoint[1].second;

        t2=clock();
        float diff ((float)t2-(float)t1);
        float seconds = diff / CLOCKS_PER_SEC;
        printf("Lama mengkomputasi hasil (di luar input&output) = %f detik \n", seconds);
        printf("Spek Komputer: Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz, 2400 Mhz, 2 Core(s), 4 Logical Processor(s), 12.0 GB Installed Physical Memory (RAM) \n");


        printf("Hasil convex hull =\n");
        cout << "(" << x1 << ", " << y1 << ") ";
        cout << "(" << x2 << ", " << y2 << ") ";

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        drawCartesian();
        glColor3f(0.0, 0.0, 1.0);

        // Set besar garis
        glLineWidth(3.0);

        glBegin(GL_LINES);
        	
        	glVertex2f ( (float) x1 / (float) 110 , (float) y1 / (float) 110 );
        	glVertex2f ( (float) x2 / (float) 110 , (float) y2 / (float) 110 );
        glEnd();
        	
        glFlush();
        drawTitik();
        glfwSwapBuffers(window);
        glfwPollEvents();

			} else {

        //Hitung titik tengah
        int titikTengahX = 0, titikTengahY = 0;
        for (int i = 0; i < (int) vectorPoint.size(); ++i)
        {
        	titikTengahX += vectorPoint[i].first;
        	titikTengahY += vectorPoint[i].second;
        }

        titikTengahX = titikTengahX / (int) vectorPoint.size();
        titikTengahY = titikTengahY / (int) vectorPoint.size();

        titikTengah = make_pair(titikTengahX,titikTengahY);

        // Mulai penyelesaian
        clock_t t1,t2;
        t1=clock();

        int indeksXMin = 0, indeksXMaks = 0;
        for (int i=1; i<banyakTitik; i++)
        {
          indeksXMin = vectorPoint[i].first < vectorPoint[indeksXMin].first? i : indeksXMin;
          indeksXMaks = vectorPoint[i].first > vectorPoint[indeksXMaks].first? i : indeksXMaks;

        }

        // Mulai cari di sisi kiri secara rekursif
        selesaikanConvexHull(vectorPoint, vectorPoint[indeksXMin], vectorPoint[indeksXMaks], 1);

        // Mulai cari di sisi kanan secara rekursif
        selesaikanConvexHull(vectorPoint, vectorPoint[indeksXMin], vectorPoint[indeksXMaks], -1);

        //Dapat hasil
        t2=clock();
        float diff ((float)t2-(float)t1);
        float seconds = diff / CLOCKS_PER_SEC;
        printf("Lama mengkomputasi hasil (di luar input&output) = %f detik \n", seconds);
        printf("Spek Komputer: Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz, 2400 Mhz, 2 Core(s), 4 Logical Processor(s), 12.0 GB Installed Physical Memory (RAM) \n");

        //Sort hasil searah jarum jam agar bisa digambar
        sort(titikHasil.begin(), titikHasil.end(), cmpSort);

        printf("Hasil convex hull =\n");
        for (int i = 0; i < (int) titikHasil.size(); ++i)
        {
          cout << "(" << titikHasil[i].first << ", " << titikHasil[i].second << ") ";
        }

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        drawCartesian();
        drawGaris();
        drawTitik();
        glfwSwapBuffers(window);
        glfwPollEvents();
			}

      printf("\nHasil convex hull telah digambar.\n");
      printf("Ulang kembali? (y/n)\n");
      cin>>perintah;
    }while(perintah == "y");

    glfwTerminate();
  }
	return 0;
}
