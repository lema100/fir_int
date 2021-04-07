#ifndef ENV_H
#define ENV_H

#include <QSettings>
#include <QString>

class env
{
public:
	env(const QString & iniFile);
	~env();

	QStringList get_coeff_list(void);
	QVector<double> get_coeff(const QString & name);
	void set_coeff(const QString & name, const QVector<double> & vector);

private:
	QSettings settings;
};

#endif // ENV_H
