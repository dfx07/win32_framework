#include "wapi.h"

____USE_NAMESPACE____


using namespace fox;

Button* btn;

void Create(WindowBase* win)
{
    //btn = new Button();
    //btn->SetLabel(L"Ngô Văn Thường");
    //btn->SetSize(200, 24);
    //btn->SetPosition(100, 100);
    //btn->SetBorderRadius(2);
    //btn->SetBorderWidth(1);
    //btn->UseEffect(true);
    //btn->Visible(true);

    //win->AddControl(btn);

    //btn = new Button();
    //btn->SetLabel(L"Ngô Văn Thường");
    //btn->SetSize(200, 24);
    //btn->SetPosition(100, 124);
    //btn->SetBorderRadius(2);
    //btn->SetBorderWidth(1);
    //btn->UseEffect(true);
    //btn->Visible(true);

    //win->AddControl(btn);

    //btn = new Button();
    //btn->SetLabel(L"Ngô Văn Thường");
    //btn->SetSize(200, 24);
    //btn->SetPosition(100, 150);
    //btn->SetBorderRadius(2);
    //btn->SetBorderWidth(1);
    //btn->UseEffect(true);
    //btn->Visible(true);

    //win->AddControl(btn);

    Checkbox* chk = new Checkbox();

    chk->SetText(L"Check box");
    chk->SetPosition(10, 40);
    chk->SetSize(100, 20);

    SubWindow* sub = new SubWindow();
    sub->SetTitle(L"sub window");
    sub->SetPosition(20, 20);
    sub->SetSize(200, 200);
    sub->Visible(true);

    //btn = new Button();
    //btn->SetLabel(L"Resize");
    //btn->SetSize(40, 20);
    //btn->SetPosition(0, 30);
    //btn->SetBorderRadius(0);
    //btn->SetBorderWidth(0);
    //btn->Visible(true);

    //sub->AddControl(btn);
    sub->AddControl(chk);

    win->AddSubWindow(sub);
    //win->AddControl(chk);

}

void MouseButton(WindowBase* win)
{
    if (win->GetMouseButtonStatus(GLMouse::LeftButton))
    {

    }
}

void Keyboard(WindowBase* win)
{
    if (win->GetKeyboardStatus(GLKeyboard::Escapex))
    {
        //win->ExitFullScreen();
    }

    else if (win->GetKeyboardStatus(GLKeyboard::KeyA))
    {

    }
}


void Draw(WindowBase* win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(120.0 / 255.0, 139.f / 255.0, 201.0 / 255.0, 1.0);
}

void Resize(WindowBase* win)
{

}

void Process(WindowBase* pWin)
{
}

int main()
{
    Window *window = new Window(L"OpenGL", 100 ,100, 640, 480);
    window->SetOnDrawfunc(Draw);
    window->SetOnCreatedfunc(Create);
    window->SetProcessfunc(Process);
    window->SetOnKeyboardfunc(Keyboard);
    window->SetOnResizefunc(Resize);

    window = create_window(window);

    while (!window->closed())
    {
        window->process();
        window->draw();
        window->wait_event();
    }


    destroy_window(window);
}
