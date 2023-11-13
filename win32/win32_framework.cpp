
#include "import_control.h"
#include "import_gl.h"
#include "xsyslogger.h"
#include "import_geo.h"
____USE_NAMESPACE____


using namespace fox;

//Button* btn;
MenuContext* menu = NULL;
Combobox* cbb;
Combobox* cbb_fun;

gp::Camera2D     cam2D;

gp::Shader       shader;

gp::GeometryRender render;


Point2D point_cur;
VecPoint2D poly_inp;
VecPolyList listpoly;

int nLineStep = 0;
Line2D line;

VecPoint2D poly_ren;

bool bShowCur = false;
int  nModeInput = -1;

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

void execute_1(WindowBase* win, Button* btn)
{

    //line.ptStart = { -71 ,-175 };
    //line.ptEnd = { 149, 89};

    //poly_inp.clear();

    //poly_inp.push_back({4,14   });
    //poly_inp.push_back({-88,-85});
    //poly_inp.push_back({36,-178});
    //poly_inp.push_back({197,-91});
    //poly_inp.push_back({214,18 });
    //poly_inp.push_back({67,-72 });
    //poly_inp.push_back({113,19 });
    //poly_inp.push_back({59,	40 });


    VecPolyList _listpoly = geo::v2::cut_line2poly(line.ptStart, line.ptEnd, poly_inp);


    listpoly.clear();
    for (int i = 0; i < _listpoly.size(); i++)
    {
        VecPoint2D poly_cut = geo::v2::poly2trig_ear_clipping(_listpoly[i]);

        listpoly.push_back(poly_cut);
    }
}


void Create(WindowBase* win)
{
    SubWindow* sub = new SubWindow();
    sub->SetSize(210, 200);

    Label* lb = new Label();
    lb->SetText(_T("Input"));
    lb->SetPosition(5, 12);
    lb->SetSize(40, 16);

    sub->AddControl(lb);

    cbb = new Combobox();
    cbb->SetPosition(5, 30);
    cbb->AddItem(_T("poly"), new int(0));
    cbb->AddItem(_T("line"), new int(1));
    cbb->AddItem(_T("point"), new int(2));
    cbb->SetSize(80, 16);
    cbb->SetEventSelectedChange([](Combobox* cbb) {

        int nSel = cbb->GetSelectIndex();

        if (nSel >= 0)
        {
            nModeInput = *static_cast<int*>(cbb->GetItemData(nSel));
        }

        nLineStep = 0;
    });

    sub->AddControl(cbb);

    cbb_fun = new Combobox();
    cbb_fun->SetPosition(90, 30);
    cbb_fun->AddItem(_T("cutLine2poly"), new int(0));
    cbb_fun->SetSize(110, 16);

    sub->AddControl(cbb_fun);

    Button* btn = new Button();
    btn->SetPosition(100, 120);
    btn->SetBorderWidth(1);
    btn->SetBorderRadius(3);
    btn->SetSize(80, 16);
    btn->SetLabel(_T("Execute"));
    btn->SetEvent(execute_1);

    sub->AddControl(btn);

    Checkbox* cb = new Checkbox();
    cb->SetSize(100, 16);
    cb->SetPosition(5, 70);
    cb->SetText(_T("show tracker cursor"));
    cb->SetCheckEvent([](Checkbox* cbx) {
        bShowCur = cbx->Checked(); 
    });

    sub->AddControl(cb);


    win->AddSubWindow(sub);

    cam2D.InitView(win->GetWidth(), win->GetHeight(), 0.1f, 1000.f);
    cam2D.SetUpCamera({ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f });

    cam2D.UpdateMatrix();

    Vec3D p1 = { -100 ,  100, 0.f };
    Vec3D p2 = {  100 ,  100, 0.f };
    Vec3D p3 = {  100 , -100, 0.f };
    Vec3D p4 = { -100 , -100, 0.f };

    Vec3D c1 = {  1 , 1, 0 };
    Vec3D c2 = {  0 , 0, 1 };

}

GLuint vaoID;
GLuint vboID;
GLuint tboID;


void CreatedDone(Window* win)
{
    //shader.Create(L"shader/vertex.glsl", L"shader/fragment.glsl");

    ////glFrontFace(GL_CCW);
    ////glCullFace(GL_FRONT);

    //glGenVertexArrays(1, &vaoID);
    //glGenBuffers(1, &vboID);
    //glGenBuffers(1, &tboID);

    //glEnable(GL_MULTISAMPLE);

    //glBindVertexArray(vaoID);
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, vboID);                                                                                 // <---.
    //    glBufferData(GL_ARRAY_BUFFER, vRectVertexDrawData.size() * sizeof(Vec3D), &vRectVertexDrawData[0].x, GL_STATIC_DRAW); //     |
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3D), (void*)0);                                             //-->--'
    //    glEnableVertexAttribArray(0);

    //    glBindBuffer(GL_ARRAY_BUFFER, tboID);                                                                               // <---.
    //    glBufferData(GL_ARRAY_BUFFER, vRectCoordDrawData.size() * sizeof(Vec2D), &vRectCoordDrawData[0], GL_STATIC_DRAW);   //     |
    //    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,sizeof(Vec2D), (void*)0);                                            //-->--'
    //    glEnableVertexAttribArray(1);
    //}
    //glBindVertexArray(0);

    float left   = -float(win->GetWidth() / 2);
    float right  =  float(win->GetWidth() / 2);
    float bottom = -float(win->GetHeight() / 2);
    float top    =  float(win->GetHeight() / 2);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(left, right, bottom, top, 0.f, -1000.f);
}



void MouseButton(WindowBase* win, int button, int action)
{
    if (button == GLMouse::LeftButton && action == GL_PRESSED)
    {
        long x, y;
        if (win->GetCursorPosCenter(x, y) == false)
            return;

        switch (nModeInput)
        {
            case 0: // add poly
            {
                //End input;
                if (poly_inp.size() >= 3 && 
                    geo::v2::is_same(poly_inp[poly_inp.size() - 2], { x, y }))
                {
                    nModeInput = -1;
                    poly_inp.pop_back();
                }

                if (poly_inp.size() <= 0)
                    poly_inp.push_back({ x, y });
                else
                    poly_inp.back() = { x, y };

                poly_ren = geo::v2::poly2trig_ear_clipping(poly_inp);
                poly_inp.push_back({ x, y });
                break;
            }
            case 1: // add line
            {
                if (nLineStep == 0)
                {
                    line.ptStart = { x, y };
                    nLineStep++;
                }
                else if(nLineStep == 1)
                {
                    line.ptEnd = { x, y };
                    nLineStep = -1;
                }

                break;
            }
            case 2: // 
            {
                break;
            }
            default:
                break;
        }
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
    Vec2D d = glm::abs(p) - b + Vec2D(r);

    return glm::min(glm::max(d.x, d.y), 0.f) + glm::length(glm::max(d, 0.f)) - r;
}

void MouseScroll(WindowBase* win)
{
    int zDelta = win->GetMouseScroll();
    float sensitive = 1.1f;
    long x, y;
    win->GetCursorPos(x, y);

    cam2D.ZoomTo(x, y, zDelta* sensitive);
    cam2D.UpdateMatrix();


    float left   = -float(win->GetWidth() / 2);
    float right  =  float(win->GetWidth() / 2);
    float bottom = -float(win->GetHeight() / 2);
    float top    =  float(win->GetHeight() / 2);

    // Projection matrix sử dụng thuộc tính zoom
    left         =  left  / cam2D.GetZoom();
    right        =  right / cam2D.GetZoom();
    bottom       =  bottom/ cam2D.GetZoom();
    top          =  top   / cam2D.GetZoom();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(left, right, bottom, top, 0.f, -1000.f);
}

Point2D p1 = { -15, -12 };
Point2D p2 = { 100, 10 };

Point2D p3 = { 20 , 15 };
Point2D p4 = { 20 , 15 };

bool bCheck = false;

void MouseMove(WindowBase* win)
{
   long x, y;
   win->GetCursorPosCenter(x, y);

   if (bShowCur)
   {
       point_cur = { x, y };
   }

   if (nLineStep > 0)
   {
       line.ptEnd = { x, y };
   }

   if (nModeInput == 0 && poly_inp.size() > 0)
   {
       poly_inp.back() = { x, y };
   }
}

float z = 10.f;

void Draw(WindowBase* win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(120.0 / 255.0, 139.f / 255.0, 201.0 / 255.0, 1.0);

    if (nLineStep > 0)
    {
        glColor3f(1.f, 1.f, 0.f);
        glBegin(GL_LINES);
        {
            glVertex2f(line.ptStart.x, line.ptStart.y);
            glVertex2f(line.ptEnd.x, line.ptEnd.y);
        }
        glEnd();
    }

    if (bShowCur)
    {
        glPointSize(5.f);
        glBegin(GL_POINTS);
        {
            glVertex2f(point_cur.x, point_cur.y);
        }
        glEnd();
    }

    if (poly_ren.size() > 0 && listpoly.size() == 0)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(Vec2D), &poly_ren[0]);
        glDrawArrays(GL_TRIANGLES, 0, poly_ren.size());

        glColor3f(1.f, 0.f, 0.f);
        glVertexPointer(2, GL_FLOAT, sizeof(Vec2D), &poly_inp[0]);
        glDrawArrays(GL_LINE_LOOP, 0, poly_inp.size());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    float color = 0.2f;
    for (int i = 0; i < listpoly.size(); i++)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(color, 0.f, 0.f);
        glVertexPointer(2, GL_FLOAT, sizeof(Vec2D), &listpoly[i][0]);
        glDrawArrays(GL_TRIANGLES, 0, listpoly[i].size());
        glDisableClientState(GL_VERTEX_ARRAY);

        color += 0.2;
    }

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

    CreatedDone(window);

    while (!window->closed())
    {
        window->process();
        window->draw();
        window->wait_event();
    }

    destroy_window(window);
}
