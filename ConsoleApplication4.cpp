#include <vtkOBJReader.h>
#include <vtkTriangleFilter.h>
#include <vtkSmartPointer.h>
#include <vtkMassProperties.h>
#include <iostream>

#include <io.h>
#include <string>
#include <vector>
#include <fstream>


using namespace std;

/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg或.obj等
*/
void getFiles(string path, vector<string>& files, const char* sType)
{
	//文件句柄
	//long hFile = 0;//Win 7
	long long hFile = 0;//Win10
	//intptr_t long hFile = 0;//Win10或Win11
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files, sType);
				}
			}
			else
			{
				char* pName = fileinfo.name;
				char* pFind = strstr(pName, sType);
				if (pFind != NULL)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}
//int i;//定义文件名索引标签变量
//char p;
//int i = 0;
int main(int argc, char** argv)
{
	vector<string> temp;
	getFiles("I://lunwen4//pe", temp, ".obj");
	std::ofstream OutFile("Test.txt");
	if (OutFile.is_open())//判断文件是否打开
	{
		for (int i = 0; i < temp.size(); i++)
		{
//      	cout << temp[i] << endl;
			vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
//    		std::string filename = temp[i];
			const char* cstr = temp[i].c_str();
			reader->SetFileName(cstr);
			reader->Update();

			vtkSmartPointer< vtkTriangleFilter > triangleFilter = vtkSmartPointer< vtkTriangleFilter >::New();
			triangleFilter->SetInputData(reader->GetOutput());
			triangleFilter->Update();

			vtkSmartPointer<vtkMassProperties> polygonProperties = vtkSmartPointer< vtkMassProperties >::New();
			polygonProperties->SetInputData(triangleFilter->GetOutput());
			polygonProperties->Update();

			double vol = polygonProperties->GetVolume();
			double area = polygonProperties->GetSurfaceArea();

			cout << temp[i] << "  " << vol << "	" << area << endl;//屏幕输出
			OutFile << temp[i] << "	" << vol << "	" << area << endl;//文件输出
		}
		OutFile.close();
//		return 0;
	}
	else
	{
		std::cerr << "无法打开文件" << std::endl;
	}
/*
	for (int n = 2; n <= 2000; n += 2)//循环索引
	{
		i = n * 10;//整10索引

		vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName("D:\\caseNoff\\20.obj");
		reader->Update();

		vtkSmartPointer< vtkTriangleFilter > triangleFilter = vtkSmartPointer< vtkTriangleFilter >::New();
		triangleFilter->SetInputData(reader->GetOutput());
		triangleFilter->Update();

		vtkSmartPointer<vtkMassProperties> polygonProperties = vtkSmartPointer< vtkMassProperties >::New();
		polygonProperties->SetInputData(triangleFilter->GetOutput());
		polygonProperties->Update();


		double vol = polygonProperties->GetVolume();
		double area = polygonProperties->GetSurfaceArea();

		cout << vol << "	" << area << endl;
	}
*/
	return 0;

}