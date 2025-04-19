#include "inc/Window.hpp"

#include "inc/DefaultScene.hpp"
#include "inc/DirectionalShadowMapScene.hpp"
#include "inc/OmnidirectionalShadowMapScene.hpp"
#include "inc/VarianceShadowMapScene.hpp"
#include "inc/ShadowVolumeScene.hpp"

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    bool debug = false;
    if (debug)
    {
        // BIl sam u znatnoj nedoumici oko koristenja *ELEMENT_ARRAY_*.
        //  Naucil sam da indices mora obavezno biti tipa UINT, a ne
        //  float ako hocu da radi. Kad bi se koristil neispravan
        //  tip, ne bi se nacrtal objekt - ali ne bi doslo do nikakve
        //  drugacije greske (malo zez za debagirat).
        // Trenutno sam stal u ShadowVolume implementaciji, di mi je
        //  sljedeci cilj probati nacrtati Volumen u 'VolumePass' koraku
        //  , koristeci glDrawElements (umjesto *Arrays) i vidit kaj dobim.
        // Mislim provjeriti dal mi nuzno treba taj glDrawElements poziv
        //  za ove potrebe, jer sa *Arrays nemrem dobiti ispravno nacrtane
        //  volumene - a nisam siguran dal je opce do *Arrays, ili je
        //  do neke greske u logiki negde drugde.
        //  Sretno buducem kumu programeru! LP!

        Window window(640, 480, "debug_prozor");

        GLfloat position_and_normal[] =
        {
             0.5,  0.5,  0.0, 0.0, 0.0, 1.0,
            -0.5,  0.5,  0.0, 0.0, 0.0, 1.0,
            -0.5, -0.5,  0.0, 0.0, 0.0, 1.0,

            -0.5, -0.5,  0.0, 0.0, 0.0, 1.0,
             0.5, -0.5,  0.0, 0.0, 0.0, 1.0,
             0.5,  0.5,  0.0, 0.0, 0.0, 1.0
        };

        GLfloat position_and_normal_old[] =
        {
             0.5,  0.5,  0.0, 0.0, 0.0, 1.0,
            -0.5,  0.5,  0.0, 0.0, 0.0, 1.0,
            -0.5, -0.5,  0.0, 0.0, 0.0, 1.0,
             0.5, -0.5,  0.0, 0.0, 0.0, 1.0,
        };

        GLuint indices[] = 
        {
            0, 1, 2,
            3, 4, 5
        };

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        GLuint VBO;
        glGenBuffers(1, &VBO);
        GLuint EBO;
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position_and_normal), position_and_normal, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (GLvoid*)0);                   // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (GLvoid*)(sizeof(float)*3));   // Normal
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        GLint buffer_size = 0xFF;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
        printf("Bafer sajzu = %d\n", buffer_size);

        const char* vertex = "shaders/default.vs";
        const char* fragment = "shaders/default.fs";
        Shader shader(vertex, fragment);
        shader.Bind();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Get(), "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.Get(), "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.Get(), "projection"), 1, GL_FALSE, &projection[0][0]);

        while (!window.ShouldClose())
        {
            window.Clear();
            window.Update();

            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            window.Draw();
        }
    }
    else
    {
        Window window(1024, 1024, "OpenGL Sjene");

        //DefaultScene default_scene(&window);
        //default_scene.Run();

        //DirectionalShadowMapScene dir_shadowmap_scene(&window);
        //dir_shadowmap_scene.Run();

        //OmnidirectionalShadowMapScene omni_shadowmap_scene(&window);
        //omni_shadowmap_scene.Run();

        //VarianceShadowMapScene var_shadowmap_scene(&window);
        //var_shadowmap_scene.Run();

        ShadowVolumeScene shadowvolume_scene(&window);
        shadowvolume_scene.Run();
    }

    return 0;
}
