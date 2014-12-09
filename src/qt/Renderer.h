#include <QtOpenGL/QtOpenGL>

class Renderer
{
    int width,height,aspect_ratio;
public:
    static Renderer* Engine();
    bool init();
    bool resize(int W,int H);
    bool draw();
private:
    Renderer(){ }
    static Renderer* m_pInstance;
};
