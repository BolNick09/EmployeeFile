#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

class IWorker 
{
public:
	virtual void printInfo() = 0;
	virtual ~IWorker() {}

	virtual double getSalary() const = 0;
};

class Employer : public IWorker 
{
private:
	std::string name;
	double salary;
public:
	Employer(const std::string& name, double salary) : name(name), salary(salary) {}

	void printInfo() override 
	{
		std::cout << "Employer: " << name << ", Salary: " << salary << std::endl;
	}

	virtual double getSalary() const override
	{
		return salary;
	}


};
class Intern : public IWorker 
{
private:
	std::string name;
	double salary;

public:
	Intern(const std::string& name, double salary) : name(name), salary(salary) {}

	void printInfo() override 
	{
		std::cout << "Intern: " << name << ", Salary: " << salary << std::endl;
	}
	virtual double getSalary() const override
	{
		return salary;
	}
};

class Boss : public IWorker 
{
private:
	std::string name;
	double salary;
	int emplCount;

public:
	Boss(const std::string& name, double salary, int emplCount) : name(name), salary(salary), emplCount(emplCount) {}

	void printInfo() override 
	{
		std::cout << "Boss: " << name << ", Salary: " << salary << ", Employees: " << emplCount << std::endl;
	}
	virtual double getSalary() const override
	{
		return salary;
	}
	int getWorkersCount() const
	{
		return emplCount;
	}


};
// Метод для чтения списка сотрудников из файла
std::vector<IWorker*> loadCompanyWorkers(const std::string& filename) 
{
	std::vector<IWorker*> companyWorkers;
	std::ifstream in(filename);
	if (!in.is_open()) 	
		throw std::runtime_error("Could not open file: " + filename);

	
	std::string line;
	while (std::getline(in, line))
	{
		//Справка для себя:
		//The class template std::basic_istringstream implements input operations on string based streams.
		//It effectively stores an instance of std::basic_string and performs input operations on it.
		std::istringstream is(line);
		std::string type;
		is >> type;
		if (type == "Employer") 
		{
			std::string name;
			double salary;
			is >> name >> salary;
			companyWorkers.push_back(new Employer(name, salary));
		}
		else if (type == "Intern") 
		{
			std::string name;
			double salary;
			is >> name >> salary;
			companyWorkers.push_back(new Intern(name, salary));
		}
		else if (type == "Boss") 
		{
			std::string name;
			double salary;
			int numEmployees;
			is >> name >> salary >> numEmployees;
			companyWorkers.push_back(new Boss(name, salary, numEmployees));
		}
	}
	in.close();
	return companyWorkers;
}


// Метод для печати информации о сотрудниках
void printCompanyWorkersInfo(const std::vector<IWorker*>& companyWorkers) 
{
	for (auto worker : companyWorkers) 
		worker->printInfo();
	
}

void calculateAverageSalaries(const std::vector<IWorker*>& companyWorkers) 
{
	double totalEmployerSalary = 0;
	int employerCount = 0;
	double totalBossSalary = 0;
	int bossCount = 0;
	double totalInternSalary = 0;
	int internCount = 0;

	for (auto worker : companyWorkers) 
	{
		if (dynamic_cast<Employer*>(worker)) 
		{
			totalEmployerSalary += worker->getSalary();
			employerCount++;
		}
		else if (dynamic_cast<Boss*>(worker)) 
		{
			totalBossSalary += worker->getSalary();
			bossCount++;
		}
		else if (dynamic_cast<Intern*>(worker)) 
		{
			totalInternSalary += worker->getSalary();
			internCount++;
		}
	}
	std::cout << "Average Employer Salary: " << (employerCount > 0 ? totalEmployerSalary / employerCount : 0) << std::endl;
	std::cout << "Average Boss Salary: " << (bossCount > 0 ? totalBossSalary / bossCount : 0) << std::endl;
	std::cout << "Average Intern Salary: " << (internCount > 0 ? totalInternSalary / internCount : 0) << std::endl;
}


// Метод для подсчета среднего количества сотрудников у боссов
void calculateAverageNumberOfCompanyWorkers(const std::vector<IWorker*>& companyWorkers) 
{
	double totalNumberOfCompanyWorkers = 0;
	int bossCount = 0;

	for (auto worker : companyWorkers) {
		if (Boss* boss = dynamic_cast<Boss*>(worker)) {
			totalNumberOfCompanyWorkers += boss->getWorkersCount();
			bossCount++;
		}
	}

	std::cout << "Average Number of CompanyWorkers per Boss: " << (bossCount > 0 ? totalNumberOfCompanyWorkers / bossCount : 0) << std::endl;
}
int main() 
{
	std::vector<IWorker*> companyWorkers;
	try 
	{
		companyWorkers = loadCompanyWorkers("companyWorkers.txt");
	}
	catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	printCompanyWorkersInfo(companyWorkers);
	std::cout << std::endl;
	calculateAverageSalaries(companyWorkers);
	//countCompanyWorkersByType(companyWorkers);

	// Опционально - посчитать среднее количество сотрудников у боссов
	calculateAverageNumberOfCompanyWorkers(companyWorkers);

	for (auto worker : companyWorkers) 
		delete worker;
	

	return 0;

}
