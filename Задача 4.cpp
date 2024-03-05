 //Задача 4

#include <iostream>
#include<thread>
#include <windows.h>
#include<mutex>

class DATA
{
public:

    DATA(int a): _a(a){}   

    int get_a() const
    {
        return	_a;
    }

        friend void swap_lock(DATA& x, DATA& y);
        friend void swap_scoped(DATA& x, DATA& y);
        friend void swap_unique(DATA& x, DATA& y);

private:
    int _a;    
    std::mutex m;
    //int _temp{ 0 };
    
};



void swap_lock(DATA& x, DATA& y)
{
    if ((x._a == y._a) ||( & x == &y))
    return; 

    std::lock(x.m,y.m);
    std::lock_guard<std::mutex> grd_x(x.m,std::adopt_lock);
    std::lock_guard<std::mutex> grd_y(y.m, std::adopt_lock); 

    int temp{ 0 };
    std::cout << "swap_lock Before: " << x._a << " " << y._a << std::endl;
    temp = x._a;
    x._a = y._a;
    y._a = temp;
    std::cout << "swap_lock After: " << x._a << " " << y._a << std::endl;
}

void swap_scoped(DATA& x, DATA& y)
{
    if ((x._a == y._a) || (&x == &y))
        return;   

    std::scoped_lock L{ x.m,y.m };    

    int temp{ 0 };
    std::cout << "swap_scoped Before: " << x._a << " " << y._a << std::endl;
    temp = x._a;
    x._a = y._a;
    y._a = temp;
    std::cout << "swap_scoped After: " << x._a << " " << y._a << std::endl;
}

void swap_unique(DATA& x, DATA& y)
{
    if ((x._a == y._a) || (&x == &y))
        return;    

    std::unique_lock<std::mutex>grd_x(x.m, std::defer_lock);
    std::unique_lock<std::mutex>grd_y(y.m, std::defer_lock);
    std::lock(grd_x, grd_y);

    int temp{ 0 };
    std::cout << "swap_unique Before: " << x._a << " " << y._a << std::endl;
    temp = x._a;
    x._a = y._a;
    y._a = temp;
    std::cout << "swap_unique After: " << x._a << " " << y._a << std::endl;
}


    int main()
    {
        setlocale(LC_ALL, "ru");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

       
        DATA A{ 7 };
        DATA B{ 5 };

        DATA C{ 9 };
        DATA D{ 4 };        
        
        std::thread t1(swap_lock, std::ref(A), std::ref(B)); 
        t1.join();
        std::thread t2(swap_scoped, std::ref(C), std::ref(D));
        t2.join();
        std::thread t3(swap_unique, std::ref(A), std::ref(B));        
        t3.join();

 return 0;
    }

