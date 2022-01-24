#include <iostream>
#include <vector>
#include <clocale>
#include <cassert>

class Motor
{
private:

    double t_os = 0;                                       //температуры окружающей среды
    int i = 0;                                             //Момент инерции двигателя I (кг∙м^2)

    //Кусочно-линейная зависимость
    std::vector<double> m;                                  //крутящий момент в Н∙м(массив)
    std::vector<double> v;                                  //скорость вращения в радиан / сек(массив)
    int t_p = 0;                                            //Температура перегрева 
    double n_m = 0;                                         //Коэффициент зависимости скорости нагрева от крутящего момента
    double n_v = 0;                                         //Коэффициент зависимости скорости нагрева от скорости вращения коленвала
    double c = 0;                                           //Коэффициент зависимости скорости охлаждения от температуры двигателя и окружающей среды
    bool p=false;
    int time = 0;                                           //время до перегрева в секундах

    double mSearch(double v_n)                              //ищем крутящий момент M от скорости вращения коленвала V
    {
        int length = v.size();
        if (v_n >= v[length - 1])
            return m[length - 1];

        for (int i = 0; i < length; i++)
        {
            if (v_n == v[i])
                return m[i];

            if ((v_n > v[i]) && (v_n < v[i + 1]))
            {
                double k = (m[i+1] - m[i]) / (v[i+1] - v[i]);

                return ((v_n - v[i]) * k + m[i]);
            };
        };

        assert("Ошибка");
        return 0;
    };
    
public:

    Motor(int create_i, std::vector<double> create_m, std::vector<double> create_v, int create_t_p, double create_n_m, double create_n_v, double create_c)
    {
        if(create_v.size()!= create_m.size())
            assert("Ошибка");

            i = create_i;
            m = create_m;
            v = create_v;
            t_p = create_t_p;
            n_m = create_n_m;
            n_v = create_n_v;
            c = create_c;
    };

    void test(double new_t_os)
    {
        t_os = new_t_os;
        time = 0;
        double t_od = t_os;                                 //температура охлаждающей житкости
        double v_n = 0;                                     //скорость вращения в радиан / сек
        double m_n = 0;                                     //крутящий момент в Н∙м
        double a = 0;                                       //ускорение

        double v_heat = 0;                                  //Скорость нагрева двигателя
        double v_cooling = 0;                               //Скорость охлаждения двигателя

        while (t_od < t_p)
        {
            time++;
            v_n += a;
            m_n = mSearch(v_n);
            a = m_n / i;

            v_heat = m_n * n_m + pow(v_n, 2) * n_v;
            v_cooling = c * (t_os - t_od);

            t_od = t_od + v_heat - v_cooling;

            if(p)
            std::cout<<"секунда "<<time<<" | Скорость вращения V "<<v_n<<" | Крутящий момент М  "<<m_n<<" | температура "<< t_od << "\n";
        };
    };

    int getTime()
    {
        return time;
    };

};



int main()
{
    setlocale(LC_CTYPE, "rus");
   
    int i = 10;                                             //Момент инерции двигателя I (кг∙м^2)
    //Кусочно-линейная зависимость
    std::vector<double> m{ 20, 75, 100, 105, 75, 0 };          //крутящий момент в Н∙м(массив)
    std::vector<double> v{ 0, 75, 150, 200, 250, 300 };        //скорость вращения в радиан / сек(массив)
    int t_p = 110;                                          //Температура перегрева 
    double n_m = 0.01;                                      //Коэффициент зависимости скорости нагрева от крутящего момента
    double n_v = 0.0001;                                    //Коэффициент зависимости скорости нагрева от скорости вращения коленвала
    double c = 0.1;                                         //Коэффициент зависимости скорости охлаждения от температуры двигателя и окружающей среды

    Motor motor1(i, m, v, t_p, n_m, n_v, c);

    

    std::cout << "Введите температуру окружающей среды ";
    double t_os;                                            //температуру окружающей среды
    std::cin >> t_os;
  
    motor1.test(t_os);
    std::cout<< "Время до перегрева в секундах " << motor1.getTime() << "\n";



    system("pause");
    return 0;
};
