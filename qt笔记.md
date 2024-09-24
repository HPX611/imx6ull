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

