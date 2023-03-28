
#include "incl_control.h"
#include "wapi.h"



____USE_NAMESPACE____


using namespace fox;

//Button* btn;
MenuContext* menu = NULL;
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





    //btn = new Button();
    //btn->SetLabel(L"Resize");
    //btn->SetSize(40, 20);
    //btn->SetPosition(0, 30);
    //btn->SetBorderRadius(0);
    //btn->SetBorderWidth(0);
    //btn->Visible(true);

    //sub->AddControl(btn);
    //sub->AddControl(chk);

    //win->AddSubWindow(sub);
    //win->AddControl(chk);

    menu = new MenuContext();
    menu->AddItem(L"Item 1", MF_STRING , NULL);
    menu->AddItem(L"Item 2", MF_STRING , NULL);
    MenuContext* mn2 = new MenuContext(L"Item 3");
    mn2->AddItem(L"Item 3.1", MF_STRING, NULL);
    mn2->AddItem(L"Item 3.2", MF_STRING, NULL);
    menu->Insert(mn2);

    win->AddControl(menu);

    SubWindow* sub = new SubWindow();
    sub->SetTitle(L"sub window");
    sub->SetPosition(20, 20);
    sub->SetSize(200, 200);
    sub->Visible(true);

    Checkbox* chk = new Checkbox();

    chk->SetText(L"Check box");
    chk->SetPosition(10, 40);
    chk->SetSize(100, 20);
    sub->AddControl(chk);

    //Button* btn = new Button();
    //btn->SetLabel(L"Resize");
    //btn->SetSize(70, 21);
    //btn->SetPosition(10, 90);
    //btn->SetBorderRadius(2);
    //btn->SetBorderWidth(1);
    //btn->Visible(true);
    //btn->UseEffect(true);
    //sub->AddControl(btn);

    //btn = new Button();
    //btn->SetLabel(L"Button 2");
    //btn->SetSize(70, 21);
    //btn->SetPosition(90, 90);
    //btn->SetBorderRadius(2);
    //btn->SetBorderWidth(1);
    //btn->Visible(true);
    //btn->UseEffect(true);
    //sub->AddControl(btn);

    Combobox* cbb = new Combobox();
    cbb->AddItem(L"select 1", new int(1));
    cbb->AddItem(L"select 2", new int(2));
    cbb->SetSelect(0);
    cbb->SetPosition(10, 60);
    cbb->SetSize(85, 20);
    sub->AddControl(cbb);

    chk = new Checkbox();
    chk->SetText(L"Check box 2");
    chk->SetPosition(10, 80);
    chk->SetSize(100, 20);
    sub->AddControl(chk);

    win->AddSubWindow(sub);
}

void MouseButton(WindowBase* win, int button, int action)
{
    if (button == GLMouse::RightButton && action == GL_PRESSED)
    {
        menu->Show({ 10 ,10 });
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
    WindowSetting setting;
    setting.m_bWriteInfo = true;
    setting.m_iModeDraw = 0;

    Window *window = new Window(L"OpenGL", 100 ,100, 640, 480, &setting);
    window->SetOnDrawfunc(Draw);
    window->SetOnCreatedfunc(Create);
    window->SetProcessfunc(Process);
    window->SetOnKeyboardfunc(Keyboard);
    window->SetOnResizefunc(Resize);
    window->SetOnMouseButtonfunc(MouseButton);

    window = create_window(window);

    while (!window->closed())
    {
        window->process();
        window->draw();
        window->wait_event();
    }

    destroy_window(window);
}
