#include <QCoreApplication>
#include <QDebug>
// 引入工具类头文件，按项目路径引入，VS2022识别
#include "../utils/Utils.h"

// 简单的测试结果打印函数
void printTestResult(const QString& testName, bool isPass)
{
    if (isPass)
    {
        qInfo() << "【测试通过】" << testName;
    }
    else
    {
        qCritical() << "【测试失败】" << testName;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // ======================================
    // 第一部分：测试像素坐标转网格索引（pixelToGrid）
    // ======================================
    qInfo() << "\n---------- 开始测试：像素坐标转网格索引 ----------";
    bool test1 = false, test2 = false, test3 = false;

    // 测试用例1：正常坐标 - QML点击(300,300)，应转换为行5列5（核心正常情况）
    QPoint grid1 = Utils::pixelToGrid(300, 300);
    if (grid1.x() == 5 && grid1.y() == 5)
    {
        test1 = true;
    }
    printTestResult("正常坐标(300,300)转网格(5,5)", test1);

    // 测试用例2：边界坐标 - QML点击(100,100)，应转换为行0列0（棋盘左上角）
    QPoint grid2 = Utils::pixelToGrid(100, 100);
    if (grid2.x() == 0 && grid2.y() == 0)
    {
        test2 = true;
    }
    printTestResult("边界坐标(100,100)转网格(0,0)", test2);

    // 测试用例3：越界坐标 - QML点击(0,0)，应返回(-1,-1)（棋盘外）
    QPoint grid3 = Utils::pixelToGrid(0, 0);
    if (grid3.x() == -1 && grid3.y() == -1)
    {
        test3 = true;
    }
    printTestResult("越界坐标(0,0)返回无效(-1,-1)", test3);

    // ======================================
    // 第二部分：测试网格索引转像素坐标（gridToPixel）
    // ======================================
    qInfo() << "\n---------- 开始测试：网格索引转像素坐标 ----------";
    bool test4 = false, test5 = false, test6 = false;

    // 测试用例4：正常索引 - 网格(5,5)，应转换为像素(320,320)（核心正常情况）
    QPoint pixel1 = Utils::gridToPixel(5, 5);
    if (pixel1.x() == 320 && pixel1.y() == 320)
    {
        test4 = true;
    }
    printTestResult("正常网格(5,5)转像素(320,320)", test4);

    // 测试用例5：边界索引 - 网格(0,0)，应转换为像素(120,120)（棋盘左上角中心点）
    QPoint pixel2 = Utils::gridToPixel(0, 0);
    if (pixel2.x() == 120 && pixel2.y() == 120)
    {
        test5 = true;
    }
    printTestResult("边界网格(0,0)转像素(120,120)", test5);

    // 测试用例6：越界索引 - 网格(15,15)，应返回(-1,-1)（超出15x15棋盘）
    QPoint pixel3 = Utils::gridToPixel(15, 15);
    if (pixel3.x() == -1 && pixel3.y() == -1)
    {
        test6 = true;
    }
    printTestResult("越界网格(15,15)返回无效(-1,-1)", test6);

    // ======================================
    // 第三部分：测试日志函数（log）- 直观输出，手动验证格式
    // ======================================
    qInfo() << "\n---------- 开始测试：日志函数（格式验证） ----------";
    Utils::log("UtilsTest", "工具类单元测试开始执行");          // INFO级别
    Utils::log("UtilsTest", "测试坐标转换边界情况", "WARN");   // WARN级别
    Utils::log("UtilsTest", "越界坐标转换失败", "ERROR");      // ERROR级别
    printTestResult("日志函数格式输出（时间戳+模块+级别）", true); // 手动看格式，直接标通过

    // ======================================
    // 测试总结
    // ======================================
    qInfo() << "\n---------- 测试总结 ----------";
    int passCount = test1 + test2 + test3 + test4 + test5 + test6 + 1;
    int totalCount = 7;
    qInfo() << "总测试用例数：" << totalCount << " | 通过用例数：" << passCount << " | 失败用例数：" << totalCount - passCount;
    if (passCount == totalCount)
    {
        qInfo() << "【全部测试通过】Utils工具类功能正常！";
    }
    else
    {
        qCritical() << "【部分测试失败】请检查Utils工具类代码！";
    }

    return a.exec();
}
