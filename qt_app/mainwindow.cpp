#include "mainwindow.h"
#include "ui_mainwindow.h"

static QValueAxis * get_my_axis(int start, int end, int tick, int minor_tick)
{
	QValueAxis *axis = new QValueAxis;

	axis->setRange(start, end);
	axis->setTickCount(tick);
	axis->setMinorTickCount(minor_tick);
	axis->setLabelFormat("%d");
	axis->setLinePenColor(QColor(Qt::GlobalColor::black));
	axis->setGridLineColor(QColor(Qt::GlobalColor::darkGray));
	axis->setMinorGridLinePen(QPen(Qt::PenStyle::DotLine));
	axis->setMinorGridLineColor(QColor(Qt::GlobalColor::darkCyan));

	return axis;
}

void MainWindow::rebuild_signal(void)
{
	auto type = ui->comboBox_signal->currentText();
	auto ampl = ui->spinBox_ampl->value() - 1;
	double value = 0;
	double phase = ui->spinBox_phase->value();
	double degree_per_sample = (double)ui->spinBox_signal->value() / (double)ui->spinBox_samplerate->value() * 360.0;

	int16_t fir_in[10000], fir_out[10000];

	for (auto i : series)
		i->clear();

	for (int i = 0; i < ui->spinBox_hor->value(); i++)
	{
		if (type == "Sine")
		{
			value = sin((phase + degree_per_sample * i) * M_PI / 180) * ampl;
		}
		else if (type == "Square")
		{
			if ((int)((phase + i * degree_per_sample) / 180) % 2)
				value = +ampl;
			else
				value = -ampl;
		}
		else if (type == "Saw")
		{
			double tmp = (phase + i * degree_per_sample) / 360;
			value = (tmp - (int)tmp) * ampl * 2 - ampl;
		}
		else if (type == "Triangle")
		{
			double tmp = (phase + i * degree_per_sample) / 360 * 2;
			if ((int)tmp % 2)
				value = (1 - (tmp - (int)tmp)) * ampl * 2 - ampl;
			else
				value = (tmp - (int)tmp) * ampl * 2 - ampl;

		}
		else if (type == "Random")
		{
			int32_t tmp = std::rand() << 15;
			tmp += std::rand();
			tmp %= ampl * 2;
			tmp -= ampl;
			value = tmp;
		}

		value += ui->spinBox_offset->value();
		if (value > ui->spinBox_vert->value())
			value = ui->spinBox_vert->value() - 1;
		else if (value < -ui->spinBox_vert->value())
			value = -ui->spinBox_vert->value() + 1;


		series.at(0)->append(i, value);
		fir_in[i] = value;
	}

	fir_int_ctx.out = fir_out;
	fir_int_ctx.len = ui->spinBox_hor->value();
	fir_int_calc(&fir_int_ctx, fir_in);

	for (int i = 0; i < ui->spinBox_hor->value(); i++)
		series.at(1)->append(i, fir_int_ctx.out[i]);
}

void MainWindow::rebuild_charts(void)
{
	auto axisX = get_my_axis(0, ui->spinBox_hor->value(), 6, 3);
	auto axisY = get_my_axis(-ui->spinBox_vert->value(), ui->spinBox_vert->value(), 5, 1);

	chart->removeAllSeries();
	series.clear();
	for (auto i : chart->axes())
	{
		chart->removeAxis(i);
		delete i;
	}

	for (uint8_t i = 0; i < ai_name.size(); i++)
	{
		series.append(new QLineSeries);
		series.last()->setName(ai_name[i]);
		series.last()->setColor(QColor(ai_color[i]));

		chart->addSeries(series.last());
		chart->setAxisX(axisX, series.last());
		chart->setAxisY(axisY, series.last());
	}
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle(windowTitle() + "   " + __DATE__);
	std::srand(std::time(nullptr));

	chart = new QChart();

	rebuild_charts();
	rebuild_signal();

	chart->legend()->setAlignment(Qt::AlignBottom);

	chart->setTitle("FIR");
	chart->setBackgroundBrush(QApplication::style()->standardPalette().brush(QPalette::Background));

	chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	ui->verticalLayout_chart->addWidget(chartView);

	ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->listView->setModel(&list_model);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_spinBox_hor_valueChanged(int i)
{
	rebuild_charts();
	rebuild_signal();
}

void MainWindow::on_spinBox_vert_valueChanged(int i)
{
	rebuild_charts();
	rebuild_signal();
}

void MainWindow::on_spinBox_samplerate_valueChanged(int i)
{
	rebuild_signal();
}

void MainWindow::on_spinBox_signal_valueChanged(int i)
{
	rebuild_signal();
}

void MainWindow::on_comboBox_signal_currentIndexChanged(int index)
{
	rebuild_signal();
}

void MainWindow::on_spinBox_ampl_valueChanged(int i)
{
	rebuild_signal();
}

void MainWindow::on_spinBox_phase_valueChanged(int i)
{
	rebuild_signal();
}

void MainWindow::on_spinBox_offset_valueChanged(int i)
{
	rebuild_signal();
}

static QVector<double> string_to_coeff(QString str)
{
	QVector<double> ret;
	bool ok = false;
	double d = 0;

	for (auto & i : str.split("\n"))
	{
		i.replace(",", ".");
		d = i.toDouble(&ok);
		if (ok)
			ret.append(d);
	}

	return ret;
}

void MainWindow::on_listView_customContextMenuRequested(QPoint p)
{
	QAction insertAct("Вставить", this);

	QObject::connect(&insertAct, &QAction::triggered, [this](void)
	{
		const QClipboard *clipboard = QApplication::clipboard();
		const QMimeData *mimeData = clipboard->mimeData();
		if (mimeData->hasText())
		{
			coeff_f = string_to_coeff(mimeData->text());
			coeff_i.resize(coeff_f.size());
			fir_int_ctx.coeff = coeff_i.data();
			fir_int_ctx.taps = coeff_f.size();
			fir_int_init(&fir_int_ctx, coeff_f.data());

			QStringList list;
			for(uint32_t i = 0; i < coeff_f.size(); i++)
				list.append(QString("k[%1]:\t%2").arg(i).arg(coeff_f.at(i)));
			list_model.setStringList(list);
			rebuild_signal();
		}
	});

	QMenu contextMenu("Context menu", this);

	contextMenu.addAction(&insertAct);
	contextMenu.exec(ui->listView->mapToGlobal(p));
}
