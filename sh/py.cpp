#include <Python.h>

void sh()
{
    Py_Initialize();              //初始化，创建一个Python虚拟环境
    if (Py_IsInitialized())
    {
        PyObject* pModule = NULL;
        PyObject* pFunc = NULL;
        pModule = PyImport_ImportModule("cam_change");  //参数为Python脚本的文件名
        if (pModule)
        {
            pFunc = PyObject_GetAttrString(pModule, "sh");   //获取函数
            PyEval_CallObject(pFunc, NULL);           //执行函数
        }
        else
        {
            printf("导入Python模块失败...\n");
        }
    }
    else
    {
        printf("Python环境初始化失败...\n");
    }
    Py_Finalize();
}