# 信号与槽

信号（Signal）就是在特定情况下被发射的事件

槽（Slot）就是对信号响应的函数，槽函数可以与一个信号关联，当信号被发射时，关联的槽函数 被自动执行，同时可以被主动的调用

## 信号和槽的关联函数

```c++
connect(sender, SIGNAL(signal()), receiver, SLOT(slot())); 
```

sender 是发射信号的对象的名称，signal() 是信号名称，信号可以看做是特殊的函 数，需要带括号，有参数时还需要指明参数，receiver 是接收信号的对象名称，slot() 是槽函数 的名称，需要带括号，有参数时还需要指明参数

* SIGNAL 和 SLOT 是 Qt 的宏，用于指明信号和槽，并将它们的参数转换为相应的字符串

* 一个信号可以连接多个槽，当一个信号与多个槽函数关联时，槽函数按照建立连接时的顺序依次执行。

* 当信号和槽函数带有参数时，在 connect()函数里，要写明参数的类型，但可以不写参数名称

* 多个信号可以连接同一个槽

* 一个信号可以连接另外一个信号
* 严格的情况下，信号与槽的参数个数和类型需要一致，至少信号的参数不能少于槽的参数。
* 只有当信号关联的所有槽函数执行完毕后，才会执行发射信号处后面的代码

![image-20240924203821260](qt%E7%AC%94%E8%AE%B0.assets/image-20240924203821260.png)

## 创建信号

```c++
#ifndef MAINWINDOW_H 
#define MAINWINDOW_H 
 
#include <QMainWindow> 
/* 引入 QPushButton */ 
#include <QPushButton> 
 
class MainWindow : public QMainWindow 
{ 
Q_OBJECT  
public: 
MainWindow(QWidget *parent = nullptr); 
~MainWindow(); 

signals: 
/* 声明一个信号，只需声明，无需定义 */ 
	void pushButtonTextChanged(); 

}; 
#endif // MAINWINDOW_H 
```

这样一个信号就定义好了

## 创建槽

```c++
#ifndef MAINWINDOW_H 
#define MAINWINDOW_H 

#include <QMainWindow> 
/* 引入 QPushButton */ 
#include <QPushButton> 

class MainWindow : public QMainWindow 
{ 
Q_OBJECT 
public: 
MainWindow(QWidget *parent = nullptr); 
~MainWindow(); 


signals: 
/* 声明一个信号，只需声明，无需定义 */ 
	void pushButtonTextChanged(); 
public slots: 
	/* 声明一个槽函数 */ 
	void changeButtonText(); 

	/* 声明按钮点击的槽函数 */ 
	void pushButtonClicked(); 

private: 
	/* 声明一个对象 pushButton */ 
	QPushButton *pushButton; 
}; 
#endif // MAINWINDOW_H 




/* 实现按钮点击槽函数 */ 
void MainWindow::pushButtonClicked() 
{ 
    /* 使用 emit 发送信号 */ 
    emit pushButtonTextChanged(); 
} 

/* 实现按钮文本改变的槽函数 */ 
void MainWindow::changeButtonText() 
{ 
    /* 在槽函数里改变按钮的文本 */ 
    pushButton->setText("被点击了！"); 
}
```

```c++
//下面就是使用这个两个将信号和槽进行连接，实现了按下按键就将txt更换的效果
connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked())); 
connect(this, SIGNAL(pushButtonTextChanged()), this, SLOT(changeButtonText())); 
```

连接信号与槽，整个流程就是当点击了按钮，然后触发了 pushButtonCli cked()，pushButtonClicked()槽里发送 pushButtonTextChanged()信号，changeBut tonText()槽响应 pushButtonTextChanged()信号，我们在 changeButtonText()槽实现 响应的动作（事件）。最终的实现效果是按钮的文本由“我是一个按钮”被点击时变成“被点击 了！”。 

-----

# QT控件

## 按键

### QPushButton 下压按钮 

写之前要引入``QPushButton``类，``#include <QPushButton> ``

### QToolButton 工具按钮

工具按钮（QToolButton） 可以带图标，通常我们在 QToolBar 这种工具条 （工具栏）上设置不同的按钮，如果这些按钮还带图标和文本，那么 QToolButton 是个不错的 选择

```c++
#ifndef MAINWINDOW_H 
#define MAINWINDOW_H 

#include <QMainWindow> 
/* 引入 QToolButton 类 */ 
#include <QToolButton> 
/* 引入 QToolBar 类 */ 
#include <QToolBar> 

class MainWindow : public QMainWindow 
{ 
    Q_OBJECT 
    
public: 
    MainWindow(QWidget *parent = nullptr); 
    ~MainWindow(); 
    
private: 
    /* 声明一个 QToolButton 对象 */ 
    QToolButton *toolButton; 
    /* 声明一个 QToolBar 对象 */ 
    QToolBar *toolBar; 
}; 
#endif // MAINWINDOW_H 
```

这个就是一个对应的.h文件，引入控件的类，声明控件对象

```c++
#include "mainwindow.h" 
#include <QApplication> 
#include <QStyle> 

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent) 
{ 
    /* 设置主窗体的位置和大小 */ 
    this->setGeometry(0, 0, 800, 480); 
    
    /* 实例化 QToolBar 对象 */ 
    toolBar = new QToolBar(this); 
    /* 设置 toolBar 的位置和大小 */ 
    toolBar->setGeometry(0, 0, 800, 100); 
    
    /* 实例化 QStyle 类对象，用于设置风格，调用系统类自带的图标 */ 
    QStyle *style = QApplication::style(); 
    
    /* 使用 Qt 自带的标准图标，可以在帮助文档里搜索 QStyle::StandardPixmap */ 
    QIcon icon = 
            style->standardIcon(QStyle::SP_TitleBarContextHelpButton); 
    
    /* 实例化 QToolButton 对象 */ 
    toolButton = new QToolButton(); 
    
    /* 设置图标 */ 
    toolButton->setIcon(icon); 
    /* 设置要显示的文本 */ 
    toolButton->setText("帮助"); 
    /* 调用 setToolButtonStyle()方法，设置 toolButoon 的样式，设置为文本置于图标下方 */ 
    toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); 
    
    /* 最后将 toolButton 添加到 ToolBar 里 */ 
    toolBar->addWidget(toolButton); 
} 

MainWindow::~MainWindow() 
{ 
}
```

这个是实例化的代码，效果如下

![image-20240924210835170](qt%E7%AC%94%E8%AE%B0.assets/image-20240924210835170.png)

### QRadioButton 单选按钮

QRadioButton 部件提供了一个带有文本标签的单选框，QRadioButton 是一个可以切换选中（checked）或未选中（unchecked）状态的选项按钮，单选框通常呈现给用户一个“多选一”的选择。也就是说，在一组单选框中，一次只能选中一 个单选框。默认在同一个父对象下，初始化后点击它们是互斥状态。

### QCheckBox 复选按钮

复选按钮（复选框）与 RadioButton 的区别是选择模式， 单选按钮提供多选一，复选按钮提供多选多

### QCommandLinkButton 命令链接按钮

CommandLinkButton 控件和 RadioButton 相似，都是用于在互斥选项中选择一项。 表面上同平面按钮一样，但是 CommandLinkButton 除带有正常的**按钮上的文字描述文本外，默 认情况下，它也将携带一个箭头图标**，表明按下按钮将打开另一个窗口或页面。

### QDialogButtonBox 

对话框和消息框通常以符合该平台界面指导原则的布局呈现按钮。不同平台的对话框总是 有不同的布局。QDialogButtonBox 允许开发人员向其添加按钮，并将自动使用适合用户桌面环 境的布局。也就是说我们可以使用系统的自带的对话框按钮，也可以自己定义对话框按钮。 



## **输入窗口部件**

### QComboBox

QComboBox 类提供了 Qt 下拉组合框的组件，QComboBox 我们常会在一些需要下拉列表选择的项目中用到，就是点击后出现下拉框，框内是添加的选项，可以进行选择

### QFontComboBox

QFontComboBox 类提供了下拉选择字体系列的组合框小部件，是一个允许用户选择字体系列 的组合框。组合框中填充了按字母顺序排列的字体家族名称列表，QFontComboBox 继承 QComboBox。

### QLineEdit

QLineEdit 小部件是一个单行文本编辑器。行编辑允许用户使用一组有用的编辑函数输入和 编辑一行纯文本。包括撤消和重做、剪切和粘贴以及拖放。通过更改行编辑的 echoMode()，它 还可以用作“只写”字段，用于输入如密码等

### QTextEdit

QTextEdit 类提供了一个查看器/编辑器小部件

### QPlainTextEdit

QPlainTextEdit 类提供了一个用于编辑和显示纯文本的小部件，常用于显示多行文本或简单 文本

QPlainTextEdit 可以理解为 QTextEdit 的低配 版。QPlainTextEdit 支持纯文本显示，QTextEdit 支持富文本（支持多种格式，比如插入图片， 链接等）显示。就是多一个样式。QPlainTextEdit 显示的效率比 QTextEdit 高，如果需要显示大 量文字，尤其是需要滚动条来回滚动的时候，QPlainTextEdit 要好很多

### QSpinBox

QSpinBox 类提供了一个微调框小部件

### QDoubleSpinBox

QDoubleSpinBox 类提供了一个用于处理浮点值微调框小部件。与 QSpinBox 作用基本一样， 与 QSpinBox 不同的是，QDoubleSpinBox 类处理的是浮点值数据

### QTimeEdit

QTimeEdit 类提供一个基于 QDateTimeEdit 类编辑时间的小部件

### QDateEdit

QDateEdit 类提供一个基于 QDateTimeEdit 类编辑时间的小部件

### QDateTimeEdit

从名字可知 QDateTimeEdit 类提供了一个用于编辑日期和时间的小部件。QDateTimeEdit 允许用户使用键盘或箭头键编辑日期，以增加或减少日期和时间值。箭头键可用于在 QDateTimeEdit 框中从一个区域移动到另一个区域。实际上是 QDateTimeEdit 和 QDateEdit 的组 合。 

### QDial

QDial 类提供了一个圆形范围控制(如速度计或电位器)。QDial 用于当用户需要在可编程定 义的范围内控制一个值，并且该范围要么是环绕的(例如，从 0 到 359 度测量的角度)，要么对 话框布局需要一个正方形小部件。由于 QDial 从 QAbstractSlider 继承，因此拨号的行为与滑块 类似。 当 wrapping（）为 false（默认设置）时，滑块和刻度盘之间没有真正的区别。 它们共 享相同的信号，插槽和成员功能。 您使用哪一个取决于您的用户期望和应用程序类型

### QScrollBar

QScrollBar 继承 QAbstractSlider。QScrollBar 小部件提供垂直或水平滚动条，允许用户访问 比用于显示文档的小部件大的文档部分。它提供了用户在文档中的当前位置和可见文档数量的 可视化指示。滚动条通常配有其他控件，可以实现更精确的导航(这里指浏览到精确的位置)。

### QSlider

QSlider 继承 QAbstractSlider。QScrollBar 类提供垂直或水平滑动条小部件，滑动条是用于 控制有界值的典型小部件。它允许用户沿着水平或垂直凹槽移动滑块手柄，并将手柄的位置转 换为合法范围内的整数值。

### QKeySequenceEdit

QKeySequenceEdit 继承 QWidget。这个小部件允许用户选择 QKeySequence, QKeySequence 通常用作快捷方式。当小部件接收到焦点并在用户释放最后一个键后一秒结束时，将启动记录， 通常用作记录快捷键

## 显示窗口部件

### QLabel

QLabel 提供了一种用于文本或图像显示的小部件

### QCalendarWidget

QCalendarWidget 继承 QWidget。QCalendarWidget 类提供了一个基于月的日历小部件，允 许用户选择日期。CalendarWidget 小部件是用当前月份和年份初始化的，QCalendarWidget 还提 供了几个公共插槽来更改显示的年份和月份。 

### QLCDNumber

QLCDNumber 继承 QFrame。QLCDNumber 小部件显示一个类似于 lcd 的数字。 QLCDNumber 小部件可以显示任意大小的数字。它可以显示十进制、十六进制、八进制或二进 制数字。使用 display()插槽很容易连接到数据源，该插槽被重载以接受五种参数类型中的任何 一种。

### QProgressBar

QProgressBar 继承 QWidget。QProgressBar 小部件提供了一个水平或垂直的进度条。进度 条用于向用户显示操作的进度，并向他们确认应用程序仍在运行

### QFrame

QFrame 继承 QWidget。QFrame 类是有框架的窗口部件的基类，它绘制框架并且调用一个 虚函drawContents()来填充这个框架。这个函数是被子类重新实现的。

这里至少还有两个有用 的函数：drawFrame()和frameChanged()。  

QPopupMenu 使用这个来把菜单“升高”，高于周围屏幕。QProgressBar 有“凹陷”的外观。 QLabel 有平坦的外观。这些有框架的窗口部件可以被改变。 

QFrame::Shape 这个枚举类型定义了 QFrame 的框架所使用的外形。当前定义的效果有：

* NoFrame - QFrame 不画任何东西
* Box - QFrame 在它的内容周围画一个框 
* Panel - QFrame 画一个平板使内容看起来凸起或者凹陷 
* WinPanel - 像 Panel，但 QFrame 绘制三维效果的方式和 Microsoft Windows 95（及其它） 的一样
* ToolBarPanel - QFrame 调用 QStyle::drawToolBarPanel() 
* MenuBarPanel - QFrame 调用 QStyle::drawMenuBarPanel() 
* HLine - QFrame 绘制一个水平线，但没有框任何东西（作为分隔是有用的）
* VLine - QFrame 绘制一个竖直线，但没有框任何东西（作为分隔是有用的）
* StyledPanel - QFrame 调用 QStyle::drawPanel()
* PopupPanel - QFrame 调用 QStyle::drawPopupPanel() 

阴影风格有：

* Plain 使用调色板的前景颜色绘制（没有任何三维效果）
* Raised 使用当前颜色组的亮和暗颜色绘制三维的凸起线。
* Sunken 使用当前颜色组的亮和暗颜色绘制三维的凹陷线。

## 显示窗口部件之浏览器

#### QTextBrowser

QTextBrowser 继承 QTextEdit，QTextBrowser 类提供了一个具有超文本导航的文本浏览器。 该类扩展了 QTextEdit(在只读模式下)，添加了一些导航功能，以便用户可以跟踪超文本文档中 的链接。
