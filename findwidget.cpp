
#include "tracer.h"
#include "findwidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QHideEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QToolButton>

QT_BEGIN_NAMESPACE

FindWidget::FindWidget(QWidget *parent)
    : QWidget(parent)
    , appPalette(qApp->palette())
{
    TRACE_OBJ
    installEventFilter(this);
    QHBoxLayout *hboxLayout = new QHBoxLayout(this);

    toolClose = setupToolButton(QLatin1String(""),tr(":/imagenes/interfaz/closetab.png"));
    hboxLayout->addWidget(toolClose);
    connect(toolClose, SIGNAL(clicked()), SLOT(hide()));

    editFind = new QLineEdit(this);
    hboxLayout->addWidget(editFind);
    editFind->setMinimumSize(QSize(150, 0));
    connect(editFind, SIGNAL(textChanged(QString)), this,
        SLOT(textChanged(QString)));
    connect(editFind, SIGNAL(returnPressed()), this, SIGNAL(findNext()));
    connect(editFind, SIGNAL(textChanged(QString)), this, SLOT(updateButtons()));

    toolPrevious = setupToolButton(tr("Previous"),tr(":/imagenes/interfaz/previous.png"));
    connect(toolPrevious, SIGNAL(clicked()), this, SIGNAL(findPrevious()));

    hboxLayout->addWidget(toolPrevious);

    toolNext = setupToolButton(tr("Next"),tr(":/imagenes/interfaz/next.png"));
    hboxLayout->addWidget(toolNext);
    connect(toolNext, SIGNAL(clicked()), this, SIGNAL(findNext()));

    checkCase = new QCheckBox(tr("Case Sensitive"), this);
    hboxLayout->addWidget(checkCase);

    labelWrapped = new QLabel(this);
    labelWrapped->setScaledContents(true);
    labelWrapped->setTextFormat(Qt::RichText);
    labelWrapped->setMinimumSize(QSize(0, 20));
    labelWrapped->setMaximumSize(QSize(105, 20));
    labelWrapped->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);
    labelWrapped->setText(tr("<img src=\":/imagenes/interfaz/wrap.png\""
        ">&nbsp;Buscar envoltura"));
    hboxLayout->addWidget(labelWrapped);

    QSpacerItem *spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding,
        QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);
    setMinimumWidth(minimumSizeHint().width());
    labelWrapped->hide();

    updateButtons();
}

FindWidget::~FindWidget()
{
    TRACE_OBJ
}

void FindWidget::show()
{
    TRACE_OBJ
    QWidget::show();
    editFind->selectAll();
    editFind->setFocus(Qt::ShortcutFocusReason);
}

void FindWidget::showAndClear()
{
    TRACE_OBJ
    show();
    editFind->clear();
}

QString FindWidget::text() const
{
    TRACE_OBJ
    return editFind->text();
}

bool FindWidget::caseSensitive() const
{
    TRACE_OBJ
    return checkCase->isChecked();
}

void FindWidget::setPalette(bool found)
{
    TRACE_OBJ
    QPalette palette = editFind->palette();
    palette.setColor(QPalette::Active, QPalette::Base, found ? Qt::white
        : QColor(255, 102, 102));
    editFind->setPalette(palette);
}

void FindWidget::setTextWrappedVisible(bool visible)
{
    TRACE_OBJ
    labelWrapped->setVisible(visible);
}

void FindWidget::hideEvent(QHideEvent* event)
{
    TRACE_OBJ
#if !defined(QT_NO_WEBKIT)
    // TODO: remove this once webkit supports setting the palette
    if (!event->spontaneous())
        qApp->setPalette(appPalette);
#else
    Q_UNUSED(event);
#endif
}

void FindWidget::showEvent(QShowEvent* event)
{
    TRACE_OBJ
#if !defined(QT_NO_WEBKIT)
    // TODO: remove this once webkit supports setting the palette
    if (!event->spontaneous()) {
        QPalette p = appPalette;
        p.setColor(QPalette::Inactive, QPalette::Highlight,
            p.color(QPalette::Active, QPalette::Highlight));
        p.setColor(QPalette::Inactive, QPalette::HighlightedText,
            p.color(QPalette::Active, QPalette::HighlightedText));
        qApp->setPalette(p);
    }
#else
    Q_UNUSED(event);
#endif
}

void FindWidget::updateButtons()
{
    TRACE_OBJ
    const bool enable = !editFind->text().isEmpty();
    toolNext->setEnabled(enable);
    toolPrevious->setEnabled(enable);
}

void FindWidget::textChanged(const QString &text)
{
    TRACE_OBJ
    emit find(text, true);
}

bool FindWidget::eventFilter(QObject *object, QEvent *e)
{
    TRACE_OBJ
    if (e->type() == QEvent::KeyPress) {
        if ((static_cast<QKeyEvent*>(e))->key() == Qt::Key_Escape) {
            hide();
            emit escapePressed();
        }
    }
    return QWidget::eventFilter(object, e);
}

QToolButton* FindWidget::setupToolButton(const QString &text, const QString &icon)
{
    TRACE_OBJ
    QToolButton *toolButton = new QToolButton(this);

    toolButton->setText(text);
    toolButton->setAutoRaise(true);
    toolButton->setIcon(QIcon(icon));
    toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    return toolButton;
}

QT_END_NAMESPACE
