
#include "import_ctrlgdi.h"
#include "import_gl.h"

____USE_NAMESPACE____


using namespace fox;

//Button* btn;
MenuContext* menu = NULL;
Camera2D     cam2D;

Shader       shader;

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

Vec2D vResolution;
Vec2D vSize;
VecPoint3D vRectVertexDrawData;
VecPoint2D vRectCoordDrawData;

void Create(WindowBase* win)
{
    ////Button* btn = new Button();
    ////btn->SetLabel(L"Button 1");
    ////btn->SetSize(100, 24);
    ////btn->SetPosition(10, 40);
    ////btn->SetBorderRadius(5);
    ////btn->SetBorderWidth(3);
    ////btn->UseEffect(true);
    ////btn->Visible(true);

    //Trackbar* track = new Trackbar();
    //track->SetSize(100, 24);
    //track->SetPosition(12, 100);
    //track->SetBorderRadius(2);
    //track->SetBorderWidth(0);
    ////btn->UseEffect(true);
    //track->Visible(true);

    //Checkbox* chk = new Checkbox();

    //chk->SetText(L"Check box");
    //chk->SetPosition(10, 30);
    //chk->SetSize(100, 20);
    //chk->SetBorderWidth(0);
    //chk->SetBorderRadius(2);

    ////menu = new MenuContext();
    ////menu->AddItem(L"Item 1", MF_STRING , NULL);
    ////menu->AddItem(L"Item 2", MF_STRING , NULL);
    ////MenuContext* mn2 = new MenuContext(L"Item 3");
    ////mn2->AddItem(L"Item 3.1", MF_STRING, NULL);
    ////mn2->AddItem(L"Item 3.2", MF_STRING, NULL);
    ////menu->Insert(mn2);

    ////win->AddControl(menu);


    //Combobox* cbb = new Combobox();
    //cbb->AddItem(L"select 1", new int(1));
    //cbb->AddItem(L"select 2", new int(2));
    //cbb->Select(0);
    //cbb->SetPosition(10, 60);
    //cbb->SetSize(85, 20);

    ////// draw input value
    ////{
    ////    Label* label = new Label();
    ////    label->SetText(L"Input value:");
    ////    label->SetPosition(5, 102);
    ////    sub->AddControl(label);

    ////    Textbox* textbox = new Textbox();
    ////    textbox->SetPosition(90, 100);
    ////    textbox->SetText(L"");
    ////    textbox->SetSize(100, 20);
    ////    sub->AddControl(textbox);
    ////}

    //SubWindow* sub = new SubWindow();
    //sub->SetTitle(L"sub window");
    //sub->SetPosition(20, 20);
    //sub->SetSize(200, 200);
    //sub->Visible(true);

    ////sub->AddControl(btn);
    //sub->AddControl(track);
    //sub->AddControl(cbb);
    //sub->AddControl(chk);
    //win->AddSubWindow(sub);
    ////Window* pWin = dynamic_cast<Window*>(win);

    ////if (pWin)
    ////{
    ////    pWin->SetCamera()
    ////}

    cam2D.InitView(win->GetWidth(), win->GetHeight(), 0.1f, 1000.f);
    cam2D.SetUpCamera({ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f });

    cam2D.UpdateMatrix();

    Vec3D p1 = { -100 ,  100, 0.f };
    Vec3D p2 = {  100 ,  100, 0.f };
    Vec3D p3 = {  100 , -100, 0.f };
    Vec3D p4 = { -100 , -100, 0.f };

    Vec3D c1 = {  1 , 1, 0 };
    Vec3D c2 = {  0 , 0, 1 };

    //render.AddLine(VERTEX_3D(p1, c1), VERTEX_3D(p2, c2));
    render.AddRectangle(p1, p2, p3, p4);
    render.UpdateRenderData();


    int width  = win->GetWidth();
    int height = win->GetHeight();

    vResolution = Vec2D(width, height);
    vSize = Vec2D(200, 200);

    Rect2D rectDraw = Rect2D(10, 10, vSize);

    Vec2D vertx1 = rectDraw.TopLeft();
    Vec2D vertx2 = rectDraw.TopRight();
    Vec2D vertx3 = rectDraw.BottomLeft();
    Vec2D vertx4 = rectDraw.BottomRight();

    vRectVertexDrawData.push_back({vertx1, 0.f}); vRectCoordDrawData.push_back({0, 1});
    vRectVertexDrawData.push_back({vertx2, 0.f}); vRectCoordDrawData.push_back({1, 1});
    vRectVertexDrawData.push_back({vertx3, 0.f}); vRectCoordDrawData.push_back({0, 0});
    vRectVertexDrawData.push_back({vertx3, 0.f}); vRectCoordDrawData.push_back({0, 0});
    vRectVertexDrawData.push_back({vertx2, 0.f}); vRectCoordDrawData.push_back({1, 1});
    vRectVertexDrawData.push_back({vertx4, 0.f}); vRectCoordDrawData.push_back({1, 0});
}

GLuint vaoID;
GLuint vboID;
GLuint tboID;

Vec2D p1;
Vec2D p2;

void CreatedDone()
{
    shader.Create(L"shader/vertex.glsl", L"shader/fragment.glsl");

    //glFrontFace(GL_CCW);
    //glCullFace(GL_FRONT);

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glGenBuffers(1, &tboID);

    glEnable(GL_MULTISAMPLE);

    glBindVertexArray(vaoID);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vboID);                                                                                 // <---.
        glBufferData(GL_ARRAY_BUFFER, vRectVertexDrawData.size() * sizeof(Vec3D), &vRectVertexDrawData[0].x, GL_STATIC_DRAW); //     |
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3D), (void*)0);                                             //-->--'
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, tboID);                                                                               // <---.
        glBufferData(GL_ARRAY_BUFFER, vRectCoordDrawData.size() * sizeof(Vec2D), &vRectCoordDrawData[0], GL_STATIC_DRAW);   //     |
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,sizeof(Vec2D), (void*)0);                                            //-->--'
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);
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

float RectSDF(Vec2D p, Vec2D b, float r)
{
    Vec2D d = abs(p) - b + Vec2D(r);
    return glm::min(glm::max(d.x, d.y), 0.f) + length(glm::max(d, 0.f)) - r;
}

void MouseScroll(WindowBase* win)
{
    int zDelta = win->GetMouseScroll();
    float sensitive = 1.1f;
    long x, y;
    win->GetCursorPos(x, y);

    cam2D.ZoomTo(x, y, zDelta* sensitive);
    cam2D.UpdateMatrix();
}

void MouseMove(WindowBase* win)
{
   long x, y;
   win->GetCursorPosCenter(x, y);

   float a= RectSDF(Vec2D(x, y) - Vec2D(0, 0), Vec2D(100, 100), 50);

   std::cout << x <<":" << y <<"=" << a << std::endl;
}

void Draw(WindowBase* win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(120.0 / 255.0, 139.f / 255.0, 201.0 / 255.0, 1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Use shader draw : vertex and fragment shader
    shader.Use();
    {
        Mat4& proj = cam2D.GetProjMatrix();
        Mat4& view = cam2D.GetViewMatrix();

        shader.SetUniformMat4("projmat", proj);
        shader.SetUniformMat4("viewmat", view);

        //shader.SetUniformVec2("resolution", vResolution);
        shader.SetUniformVec2("btn_size", vSize);

        glBindVertexArray(vaoID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBegin(GL_LINE);
    {

    }
    glEnd();

    //cam2D.UseMatrix();
    //render.Draw();
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
    setting.m_iAntialiasing = 2;

    Window *window = new Window(L"OpenGL", 100 ,100, 640, 480, &setting);
    window->SetOnDrawfunc(Draw);
    window->SetOnCreatedfunc(Create);
    window->SetProcessfunc(Process);
    window->SetOnKeyboardfunc(Keyboard);
    window->SetOnResizefunc(Resize);
    window->SetOnMouseButtonfunc(MouseButton);
    window->SetOnMouseScrollfunc(MouseScroll);
    window->SetOnMouseMovefunc(MouseMove);

    window = create_window(window);

    CreatedDone();

    while (!window->closed())
    {
        window->process();
        window->draw();
        window->wait_event();
    }

    destroy_window(window);
}
