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

### QGraphicsView

QGraphicsView 继承 QAbstractScrollArea。QGraphicsView 是图形视图框架的一部分，它提 供了基于图元的模型/视图编程。QGraphicsView 在可滚动视图中可视化 QGraphicsScene 的内容。 要创建带有几何项的场景，请参阅 QGraphicsScene 的文档。 

要可视化场景，首先构造一个 QGraphicsView 对象，将要可视化的场景的地址传递给 QGr aphicsView 的构造函数。或者，可以调用 setScene()在稍后设置场景。

## 布局管理

布局管理控件继承关系

![image-20240926145832202](qt%E7%AC%94%E8%AE%B0.assets/image-20240926145832202.png)

* QBoxLayout 提供了水平和垂直的布局管理；
* QFormLayout 提供了将输入部件和标签 成组排列的布局管理；
* QGridLayout 提供了网格形式的布局管理；
* QStackedLayout 提供了一组 布局后的部件，可以对它们进行分布显示

### QBoxLayout

QBoxLayout 继承 QLayout。QBoxLayout 类提供水平或垂直地排列子部件。QBoxLayout 获 取从它的父布局或从 parentWidget()中所获得的空间，将其分成一列框，并使每个托管小部件填 充一个框。 

### QGridLayout

QGridLayout 类提供了布局管理器里的一种以网格（二维）的方式管理界面组件，以按钮 组件为例，它们所对应网格的坐标下表，与二维数组类似。 

![image-20240926150424499](qt%E7%AC%94%E8%AE%B0.assets/image-20240926150424499.png)

QGridLayout继承QLayout。QGridLayout获取可用的空间(通过其父布局或parentWidget()))， 将其分为行和列，并将其管理的每个小部件放入正确的单元格中。由于网格布局管理器中的组 件也是会随着窗口拉伸而发生变化的，所以也是需要设置组件之间的比例系数的，与 QBoxLayout 不同的是网格布局管理器还需要分别设置行和列的比例系数。

### QFormLayout

QFormLayout 继承 QLayout。QFormLayout 类管理输入小部件及其关联标签的表单。QFor mLayout 是一个方便的布局类，它以两列的形式布局其子类。左列由标签组成，右列由“字段” 小部件(QLineEdit(行编辑器)、QSpinBox(旋转框等))组成。通常使用 setRowWrapPolicy(RowWr apPolicy policy)接口函数设置布局的换行策略进行布局等。 



## 空间间隔

QSpacerItem 继承 QLayoutItem。QSpacerItem 类在布局中提供空白(空间间隔)。所以 QSpacerItem 是在布局中使用的。它包含 Horizontal Spacer（水平间隔）与 Vertical Spacer（垂直 间隔）。 

### QSpacerItem

QSpacerItem 继承 QLayoutItem。QSpacerItem 类在布局中提供空白(空间间隔)。所以 QSpa cerItem 是在布局中使用的。



##  容器

### QGroupBox

QGroupBox 小部件提供一个带有标题的组框框架。一般与一组或者是同类型的部件一起使 用。 布局时可用作一组控 件的容器，但是需要注意的是，内部通常使用布局控件（如 QBoxLayout）进行布局

### QScrollArea

滚动区域用于在框架中显示子部件的内容。如果小 部件超过框架的大小，视图就会出现滚动条，以便可以查看子小部件的整个区域。

### QToolBox

QToolBox 类提供了一列选项卡小部件项。工具箱是一个小部件， 它显示一列选项卡在另一列的上面，当前项显示在当前选项卡的下面。每个选项卡在选项卡列 中都有一个索引位置。选项卡的项是 QWidget。

### QTabWidget

QTabWidget 继承 QWidget，QTabWidget 类提供了一组选项卡（多页面）小部件。QTabWi dget 主要是用来分页显示的，每一页一个界面，众多界面公用一块区域，节省了界面大小，很 方便的为用户显示更多的信息。类似浏览器的多标签页面，所以这个控件在实际项目中也会经 常用到。

### QStackedWidget

QStackedWidget 继承 QFrame。QStackedWidget 类提供了一个小部件堆栈，其中一次只能 看到一个小部件，与 QQ 的设置面板类似。

QStackedWidget 可 用 于 创 建 类 似 于 QTabWidget 提 供 的 用 户 界 面 。 它 是 构 建 在 QStackedLayout 类之上的一个方便的布局小部件。常与 QListWidget 搭配使用，效果如下图， 左边的是 QListWidget 列表，右边的是 QStackedWidget。他们一般与信号槽连接，通过点击左 边的 QListWidget 列表，使用信号槽连接后，就可以让右边的 QStackedWidget 显示不同的内容， 每次显示一个 widget 小部件。 

### QFrame

### QWidget

QWidget 类是所有用户界面对象的基类（如 QLabel 类继承于 QFrame 类，而 QFrame 类又 继承于 QWidget 类）。Widget 是用户界面的基本单元：它从窗口系统接收鼠标，键盘和其他事 件，并在屏幕上绘制自己。每个 Widget 都是矩形的，它们按照 Z-order 进行排序。 

注：Z-order 是重叠二维对象的顺序，例如堆叠窗口管理器中的窗口。典型的 GUI 的特征 之一是窗口可能重叠，使得一个窗口隐藏另一个窗口的一部分或全部。 当两个窗口重叠时，它 们的 Z 顺序确定哪个窗口出现在另一个窗口的顶部。

理解：术语"z-order"指沿着 z 轴物体的顺序。三维坐标轴中 x 横轴，y 数轴，z 上下轴。可 以将 gui 窗口视为平行与显示平面的一系列平面。因此，窗口沿着 z 轴堆叠。所以 z-order 指定 了窗口的前后顺序。就像您桌面上的一叠纸一样，每张纸是一个窗口，桌面是您的屏幕，最上 面的窗口 z 值最高。 

QWidget 不是一个抽象类，它可以用作其他 Widget 的容器，并很容易作为子类来创建定制 Widget。它经常用于创建、放置和容纳其他的 Widget 窗口。 

### QMdiArea

QMdiArea 继承 QAbstractScrollArea。QMdiArea 小部件提供一个显示 MDI 窗口的区域。  QMdiArea的功能本质上类似于MDI窗口的窗口管理器。大多数复杂的程序，都使用MDI框架， 在 Qt designer 中可以直接将控件 MDI Area 拖入使用。 

### QDockWidget

QDockWidget继承QWidget。QDockWidget类提供了一个小部件，可以停靠在QMainWindow 内，也可以作为桌面的顶级窗口浮动。 

QDockWidget 提供了停靠部件的概念，也称为工具面板或实用程序窗口。停靠窗口是放置 在 QMainWindow 中央窗口附近的停靠窗口部件区域中的辅助窗口。停靠窗口可以被移动到当 前区域内，移动到新的区域，并由终端用户浮动(例如，不停靠)。QDockWidget API 允许程序员 限制 dock widget 的移动、浮动和关闭能力，以及它们可以放置的区域。QDockWidget 的初始停 靠区域有 Qt.BottomDockWidgetArea（底部停靠）、Qt.LeftDockWidgetArea（左边停靠、； Qt.RightDockWidgetArea （ 右 边 停 靠 ）、 Qt.TopDockWidgetArea （ 顶 部 停 靠 ） 和 Qt.NoDockWidgetArea（不显示 Widget）。



## 项目视图组

### QListView

QListView 继承 QAbstractItemView，被 QListWidget 和 QUndoView 继承。QListView 类提 供模型上的列表或图标视图。QListView 以简单的非分层列表或图标集合的形式显示存储在模 型中的项。该类用于提供以前由 QListBox 和 QIconView 类提供的列表和图标视图，但是使用 了 Qt 的模型/视图体系结构提供的更灵活的方法。QListView 是基于 Model，而 QListWidget 是 基于 Item，这是它们的本质区别。QListView 需要建模，所以减少了冗余的数据，程序效率高; 而 QListWidget 是一个升级版本的 QListView（实质是封装好了 model 的 QListView），它已经自 己为我们建立了一个数据存储模型（QListWidgetItem），操作方便，直接调用 addItem 即可添加 项目（ICON，文字）。

QT 提供了一些现成的 models 用于处理数据项（这些是 Qt 处理数据模型的精华，如果用到 Qt 数据模型，下面这些是经常要用到的）：

* QStringListModel 用于存储简单的 QString 列表。
* QStandardItemModel 管理复杂的树型结构数据项，每项都可以包含任意数据。
* QDirModel 提供本地文件系统中的文件与目录信息。
* QSqlQueryModel, QSqlTableModel,QSqlRelationTableModel 用来访问数据库。

### QTreeView

QTreeView 继承 QAbstractItemView，被 QTreeWidget 继承。QTreeView 类提供树视图的默 认模型/视图实现。QTreeView 实现了模型项的树表示。该类用于提供以前由 QListView 类提供 的标准分层列表，但是使用了 Qt 的模型/视图体系结构提供的更灵活的方法。 

### QTableView

QTableView 继承 QAbstractItemView，被 QTableWidget 继承。QTableView 类提供了表视图 的默认模型/视图实现。QTableView 实现了一个表视图，用于显示来自模型的项。该类用于提 供以前由 QTable 类提供的标准表，但使用 Qt 的模型/视图体系结构提供的更灵活的方法。

### QColumnView

QColumnView 继承 QAbstractItemView。QColumnView 在许多 QListViews 中显示一个模型， 每个 QListViews 对应树中的每个层次结构。这有时被称为级联列表。QColumnView 类是模型/ 视图类之一，是 Qt 模型/视图框架的一部分。QColumnView 实现了由 QAbstractItemView 类定 义的接口，以允许它显示由派生自 QAbstractItemModel 类的模型提供的数据。

### QUndoView

QUndoView 继承 QlistView。QUndoView 类显示 QUndoStack 的内容。QUndoView 是一个 QListView，它显示在撤销堆栈上推送的命令列表。总是选择最近执行的命令。选择不同的命令 会导致调用QUndoStack::setIndex()，将文档的状态向后或向前滚动到新命令。可以使用setStack() 显式地设置堆栈。或者，可以使用 setGroup()来设置 QUndoGroup 对象。当组的活动堆栈发生 变化时，视图将自动更新自身。

## 项目控件组

### QListWidget

QListWidget 继承 QListView。QListWidget 类提供了一个基于项的列表小部件。QListWidg et 是一个便捷的类，它提供了一个类似于 QListView（下一小节将讲到）提供的列表视图，但 是提供了一个用于添加和删除项目的基于项目的经典接口。QListWidget 使用内部模型来管理列 表中的每个 QListWidgetItem。 

### QTreeWidget

QTreeWidget 继承 QTreeView。QTreeWidget 类提供了一个使用预定义树模型的树视图。  QTreeWidget 类是一个便捷的类，它提供了一个标准的树小部件，具有一个类似于 qt3 中的 QLi stView 类所使用的基于项目的经典接口。该类基于 Qt 的模型/视图体系结构，并使用默认模型 来保存项，每个项都是 QTreeWidgetItem。

### QTableWidget

QTableWidget 继承 QTableView。QTableWidget 类提供了一个带有默认模型的基于项的表视 图。表小部件为应用程序提供标准的表显示工具。QTableWidget 中的项由 QTableWidgetItem 提 供。 



----



# 文本读写

## QFile

QFile 类提供了读取和写入文件的接口。在嵌入式里如果需要读写文件，最简单的方法就是 用 Qfile。

QFile 是一个读写文本、二进制文件和资源的 I/O 设备。QFile 可以自己使用，也可以更方 便地与 QTextStream 或 QDataStream 一起使用。

文件名通常在构造函数中传递，但它可以在任何时候使用 setFileName()设置。不支持使用 其他分隔符(例如'\')。所以在 Windows、 Linux 或者 Mac 里文件的路径都是用'/'。不能看到 Windows 的路径是'\'，我们就可以在写入的文件路径里添加这个'\'。不管操作系统是什么，QFile 的文件分隔符都是'/'。 

可以使用 exists()检查文件是否存在，并使用 remove()删除文件。(更高级的文件系统相关操 作由 QFileInfo 和 QDir 提供。)用 open()打开文件，用 close()关闭文件，用 flush()刷新文件。通 常使用 QDataStream 或 QTextStream 读写数据，但也可以调用 QIODevice 继承的函数 read()、 readLine()、readAll()、write()。QFile 还继承 getChar()、putChar()和 ungetChar()，它们一次只处 理一个字符。文件的大小由 size()返回。可以使用 pos()获取当前文件位置，也可以使用 seek() 移动到新的文件位置。如果已经到达文件的末尾，则 atEnd()返回 true。 

QFile::open()函数打开文件时需要传递 QIODevice::OpenModeFlag 枚举类型的参数，决定 文件以什么方式打开，QIODevice::OpenModeFlag 类型的主要取值如下： 

* QIODevice::ReadOnly：以只读方式打开文件，用于载入文件。 
* QIODevice::WriteOnly：以只写方式打开文件，用于保存文件。
* QIODevice::ReadWrite：以读写方式打开。 
* QIODevice::Append：以添加模式打开，新写入文件的数据添加到文件尾部。 
* QIODevice::Truncate：以截取方式打开文件，文件原有的内容全部被删除。 
* QIODevice::Text：以文本方式打开文件，读取时“\n”被自动翻译为换行符，写入时字符 串结束符会自动翻译为系统平台的编码，如 Windows 平台下是“\r\n”。

### QTextStream **读写文本** 

QTextStream 类为读写文本提供了一个方便的接口，常与 QFile 结合使用。QTextStream 可 以在 QIODevice、QByteArray 或 QString 上操作。使用 QTextStream 的流操作符，您可以方便 地读写单词、行和数字。为了生成文本，QTextStream 支持字段填充和对齐的格式化选项，以 及数字的格式化。看到 Stream 这个名词就知道，它与流操作有关，那么我们可以使用 C++的操 作符“<<”和“>>”(流提取运算符和流插入运算符)进行操作流了。 

----

# 多线程

## **继承** QThread **的线程** 

**![image-20240927145402706](qt%E7%AC%94%E8%AE%B0.assets/image-20240927145402706.png)**

通过上面的图我们可以看到，主线程内有很多方法在主线程内，但是子线程，只有 run() 方法是在子线程里的。run()方法是继承于 QThread 类的方法，用户需要重写这个方法，一般是 把耗时的操作写在这个 run()方法里面。 

```c++
/* 新建一个 WorkerThread 类继承于 QThread */
class WorkerThread : public QThread
{
    /* 用到信号槽即需要此宏定义 */
    Q_OBJECT

public:
    WorkerThread(QWidget *parent = nullptr) {
        Q_UNUSED(parent);
    }

    /* 重写 run 方法，继承 QThread 的类，只有 run 方法是在新的线程里 */
    void run() override {
        QString result = "线程开启成功";

        /* 这里写上比较耗时的操作 */
        // ...
        // 延时 2s，把延时 2s 当作耗时操作
        sleep(2);

        /* 发送结果准备好的信号 */
        emit resultReady(result);
    }

signals:
    /* 声明一个信号，译结果准确好的信号 */
    void resultReady(const QString &s);
};


void MainWindow::handleResults(const QString &result)
{
    /* 打印出线程发送过来的结果 */
    qDebug()<<result<<endl;
}

void MainWindow::pushButtonClicked()
{
    /* 检查线程是否在运行，如果没有则开始运行 */
    if (!workerThread->isRunning())
        workerThread->start();
}
```

主要是新建一个WorkerThread，通过信号和槽去开启或关闭这个线程，线程里只能重载run函数去是实现自己真正要做的事情

### **继承** QObject **的线程**

继承 QThread 类是创建线程的一种方法，另一种就是继承 QObject 类。继承 QObject 类更加灵活。它通过 QObject::moveToThread()方法，将一个 QObeject 的类转移到一个线程里执行。

![image-20240927152540550](qt%E7%AC%94%E8%AE%B0.assets/image-20240927152540550.png)

通过上面的图不难理解，首先我们写一个类继承 QObject，通过 QObject::moveToThread() 方法将它移到一个 QThread 线程里执行。那么可以通过主线程发送信号去调用 QThread 线程的 方法如上图的 fun4()，fun5()等等。这些方法都是在 QThread 线程里执行的。 
