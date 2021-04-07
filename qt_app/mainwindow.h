#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../fir_int/fir_int.h"
#include "env.h"
#include <QMainWindow>
#include <QtCharts>
#include <cmath>

const QVector<QString> ai_name =
{
	"Input",
	"Output",
};

const QVector<Qt::GlobalColor> ai_color =
{
	Qt::GlobalColor::red,
	Qt::GlobalColor::darkRed,
	Qt::GlobalColor::blue,
	Qt::GlobalColor::darkBlue,
	Qt::GlobalColor::green,
	Qt::GlobalColor::darkGreen,
	Qt::GlobalColor::magenta,
	Qt::GlobalColor::darkMagenta,
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	void rebuild_charts(void);
	void rebuild_signal(void);

	Ui::MainWindow *ui;
	QVector<QLineSeries *> series;
	QChartView *chartView;
	QChart *chart;
	QTimer *timer;
	QStringListModel list_model;

	QVector<double> coeff_f;
	QVector<int32_t> coeff_i;
	fir_int_ctx_t fir_int_ctx;
	env _env;

public slots:
	void on_spinBox_hor_valueChanged(int i);
	void on_spinBox_vert_valueChanged(int i);
	void on_spinBox_samplerate_valueChanged(int i);
	void on_spinBox_signal_valueChanged(int i);
	void on_spinBox_ampl_valueChanged(int i);
	void on_spinBox_phase_valueChanged(int i);
	void on_spinBox_offset_valueChanged(int i);
	void on_comboBox_signal_currentIndexChanged(int index);
	void on_listView_customContextMenuRequested(QPoint p);
	void on_checkBox_presamp_stateChanged(int state);
};

#endif // MAINWINDOW_H
