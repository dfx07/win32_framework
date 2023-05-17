
#include "import_control.h"
#include "import_gl.h"

____USE_NAMESPACE____


using namespace fox;

//Button* btn;
MenuContext* menu = NULL;
Camera2D     cam2D;

GeometryRender render;

struct Component
{
    int   index;
    float x;
    float y;
    float width;
    float height;
};

typedef std::vector < Component>  ComponentList;

ComponentList ReadFile(const char* filedata)
{
    ComponentList comp_list;

    FILE* file = NULL;
    if (fopen_s(&file, filedata, "r") == 0)
    {
        int index;
        float x, y, width, height;

        const int iNumberField = 5;

        while (!feof(file))
        {
            if(iNumberField == fscanf_s(file, "%d,%f,%f,%f,%f", &index, &x, &y, &width, &height))
            {
                comp_list.emplace_back(Component{ index, x, y , width, height });
            }
        }
    }

    fclose(file);

    return comp_list;
}

void Create(WindowBase* win)
{
    //Button* btn = new Button();
    //btn->SetLabel(L"Button 1");
    //btn->SetSize(100, 24);
    //btn->SetPosition(10, 40);
    //btn->SetBorderRadius(5);
    //btn->SetBorderWidth(3);
    //btn->UseEffect(true);
    //btn->Visible(true);

    Trackbar* track = new Trackbar();
    track->SetSize(100, 24);
    track->SetPosition(12, 100);
    track->SetBorderRadius(2);
    track->SetBorderWidth(0);
    //btn->UseEffect(true);
    track->Visible(true);

    Checkbox* chk = new Checkbox();

    chk->SetText(L"Check box");
    chk->SetPosition(10, 30);
    chk->SetSize(100, 20);
    chk->SetBorderWidth(0);
    chk->SetBorderRadius(2);

    //menu = new MenuContext();
    //menu->AddItem(L"Item 1", MF_STRING , NULL);
    //menu->AddItem(L"Item 2", MF_STRING , NULL);
    //MenuContext* mn2 = new MenuContext(L"Item 3");
    //mn2->AddItem(L"Item 3.1", MF_STRING, NULL);
    //mn2->AddItem(L"Item 3.2", MF_STRING, NULL);
    //menu->Insert(mn2);

    //win->AddControl(menu);


    Combobox* cbb = new Combobox();
    cbb->AddItem(L"select 1", new int(1));
    cbb->AddItem(L"select 2", new int(2));
    cbb->Select(0);
    cbb->SetPosition(10, 60);
    cbb->SetSize(85, 20);

    //// draw input value
    //{
    //    Label* label = new Label();
    //    label->SetText(L"Input value:");
    //    label->SetPosition(5, 102);
    //    sub->AddControl(label);

    //    Textbox* textbox = new Textbox();
    //    textbox->SetPosition(90, 100);
    //    textbox->SetText(L"");
    //    textbox->SetSize(100, 20);
    //    sub->AddControl(textbox);
    //}

    SubWindow* sub = new SubWindow();
    sub->SetTitle(L"sub window");
    sub->SetPosition(20, 20);
    sub->SetSize(200, 200);
    sub->Visible(true);

    //sub->AddControl(btn);
    sub->AddControl(track);
    sub->AddControl(cbb);
    sub->AddControl(chk);
    win->AddSubWindow(sub);
    //Window* pWin = dynamic_cast<Window*>(win);

    //if (pWin)
    //{
    //    pWin->SetCamera()
    //}

    cam2D.InitView(win->GetWidth(), win->GetHeight(), 0.1f, 1000.f);
    cam2D.SetUpCamera({ 0.f, 0.f, 8.f }, { 0.f, 0.f, 1.f });

    cam2D.UpdateMatrix();


    glm::vec3 p1 = { -100 ,  100, 0.f };
    glm::vec3 p2 = {  100 ,  100, 0.f };
    glm::vec3 p3 = {  100 , -100, 0.f };
    glm::vec3 p4 = { -100 , -100, 0.f };

    glm::vec3 c1 = {  1 , 1, 0 };
    glm::vec3 c2 = {  0 , 0, 1 };

    //render.AddLine(VERTEX_3D(p1, c1), VERTEX_3D(p2, c2));
    render.AddRectangle(p1, p2, p3, p4);
    render.UpdateRenderData();
}

void MouseButton(WindowBase* win, int button, int action)
{
    if (button == GLMouse::RightButton && action == GL_PRESSED)
    {
        //menu->Show({ 10 ,10 });
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

    cam2D.UseMatrix();

    render.Draw();
}

void Resize(WindowBase* win)
{
    cam2D.SetViewSize(win->GetWidth(), win->GetHeight());
    cam2D.UpdateMatrix();
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
