#include "env.h"

env::env(const QString & iniFile) : settings(iniFile + ".ini", QSettings::IniFormat, nullptr)
{

}

env::~env()
{

}

QStringList env::get_coeff_list(void)
{
	settings.beginGroup("COEFFICIENTS");
	auto ret = settings.allKeys();
	settings.endGroup();

	return ret;
}

QVector<double> env::get_coeff(const QString & name)
{
	settings.beginGroup("COEFFICIENTS");
	auto str = settings.value(name, "").toString();
	settings.endGroup();

	QVector<double> ret;
	bool ok = false;
	double d = 0;

	for (auto & i : str.split(";"))
	{
		i.replace(",", ".");
		d = i.toDouble(&ok);
		if (ok)
			ret.append(d);
	}

	return ret;
}

void env::set_coeff(const QString & name, const QVector<double> & vector)
{
	QStringList tmp;

	for (auto & i : vector)
		tmp.append(QString::number(i));

	settings.beginGroup("COEFFICIENTS");
	settings.setValue(name, tmp.join(";"));
	settings.endGroup();
}
