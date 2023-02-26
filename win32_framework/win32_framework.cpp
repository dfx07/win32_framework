#include <xsysutils.h>
#include <xeasing.h>


____USE_NAMESPACE____

int main()
{
    xMonitorInfo mn = fox::get_monitorinfo();

    fox::EasingEngine ease;
    ease.AddExec(EaseType::Back, EaseMode::In, 100, 2000);

    ease.Start();
    ease.Update(0.1);
    double a = ease.Value(0);


    getchar();

    return 1;
}