#include "Level.h"
#include "Map.h"
#include <QFile>
#include <QTextStream>

Level::Level(int ID)
{
    levelID=ID;
    LevelNum = 5;
    if(ID==1){
        initcosts=300;
        Homehp=3;
        MAX_enemies=10;
        for(int i=1;i<=3;i++)spawnQueue.push({1,0});
        for(int i=1;i<=4;i++)spawnQueue.push({2,0});
        for(int i=1;i<=3;i++)spawnQueue.push({3,0});
        for(int i=0;i<=MAP_ROWS;i++)
            for(int j=0;j<=MAP_COLS;j++)
                l_gridmap[i][j]=0;

        QFile file(":/assets/level_1.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            int row = 1;
            while(!in.atEnd() && row <= MAP_ROWS){
                QString line = in.readLine();
                for(int col = 1; col <= MAP_COLS && (col-1) < line.size(); col++){
                    QChar ch = line[col-1];
                    if(ch == '1'){
                        l_gridmap[row][col] = 1;
                    } else if(ch == 'S'){
                        l_gridmap[row][col] = 3;
                        startpoints.push_back({row, col});
                    } else if(ch == 'E'){
                        l_gridmap[row][col] = 4;
                        endpoint = {row, col};
                    } else {
                        l_gridmap[row][col] = 0;
                    }
                }
                row++;
            }
            file.close();
        } else {
            qDebug() << "Failed to open level_1.txt";
        }
    }
    else if(ID==2){
        initcosts=300;
        Homehp=3;
        MAX_enemies=15;
        for(int i=1;i<=3;i++)spawnQueue.push({2,0});
        for(int i=1;i<=3;i++)spawnQueue.push({2,1});
        for(int i=1;i<=3;i++)spawnQueue.push({2,2});
        for(int i=1;i<=2;i++)spawnQueue.push({3,0});
        for(int i=1;i<=2;i++)spawnQueue.push({3,1});
        for(int i=1;i<=2;i++)spawnQueue.push({3,2});
        for(int i=0;i<=MAP_ROWS;i++)
            for(int j=0;j<=MAP_COLS;j++)
                l_gridmap[i][j]=0;

        QFile file(":/assets/level_2.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            int row = 1;
            while(!in.atEnd() && row <= MAP_ROWS){
                QString line = in.readLine();
                for(int col = 1; col <= MAP_COLS && (col-1) < line.size(); col++){
                    QChar ch = line[col-1];
                    if(ch == '1'){
                        l_gridmap[row][col] = 1;
                    } else if(ch == 'S'){
                        l_gridmap[row][col] = 3;
                        startpoints.push_back({row, col});
                    } else if(ch == 'E'){
                        l_gridmap[row][col] = 4;
                        endpoint = {row, col};
                    } else {
                        l_gridmap[row][col] = 0;
                    }
                }
                row++;
            }
            file.close();
        } else {
            qDebug() << "Failed to open level_1.txt";
        }
    }
    else if(ID==3){
        initcosts=500;
        Homehp=2;
        MAX_enemies=20;
        for(int i=1;i<=4;i++)spawnQueue.push({1,0});
        for(int i=1;i<=4;i++)spawnQueue.push({1,1});
        for(int i=1;i<=4;i++)spawnQueue.push({1,2});
        for(int i=1;i<=3;i++)spawnQueue.push({2,0});
        for(int i=1;i<=3;i++)spawnQueue.push({2,1});
        for(int i=1;i<=2;i++)spawnQueue.push({3,2});
        for(int i=0;i<=MAP_ROWS;i++)
            for(int j=0;j<=MAP_COLS;j++)
                l_gridmap[i][j]=0;

        QFile file(":/assets/level_3.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            int row = 1;
            while(!in.atEnd() && row <= MAP_ROWS){
                QString line = in.readLine();
                for(int col = 1; col <= MAP_COLS && (col-1) < line.size(); col++){
                    QChar ch = line[col-1];
                    if(ch == '1'){
                        l_gridmap[row][col] = 1;
                    } else if(ch == 'S'){
                        l_gridmap[row][col] = 3;
                        startpoints.push_back({row, col});
                    } else if(ch == 'E'){
                        l_gridmap[row][col] = 4;
                        endpoint = {row, col};
                    } else {
                        l_gridmap[row][col] = 0;
                    }
                }
                row++;
            }
            file.close();
        } else {
            qDebug() << "Failed to open level_1.txt";
        }
    }
    else if(ID==4){
        initcosts=500;
        Homehp=2;
        MAX_enemies=20;
        for(int i=1;i<=4;i++)spawnQueue.push({2,0});
        for(int i=1;i<=3;i++)spawnQueue.push({1,1});
        for(int i=1;i<=3;i++)spawnQueue.push({1,2});
        for(int i=1;i<=3;i++)spawnQueue.push({2,3});
        for(int i=1;i<=3;i++)spawnQueue.push({2,4});
        for(int i=1;i<=4;i++)spawnQueue.push({3,5});
        for(int i=0;i<=MAP_ROWS;i++)
            for(int j=0;j<=MAP_COLS;j++)
                l_gridmap[i][j]=0;

        QFile file(":/assets/level_4.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            int row = 1;
            while(!in.atEnd() && row <= MAP_ROWS){
                QString line = in.readLine();
                for(int col = 1; col <= MAP_COLS && (col-1) < line.size(); col++){
                    QChar ch = line[col-1];
                    if(ch == '1'){
                        l_gridmap[row][col] = 1;
                    } else if(ch == 'S'){
                        l_gridmap[row][col] = 3;
                        startpoints.push_back({row, col});
                    } else if(ch == 'E'){
                        l_gridmap[row][col] = 4;
                        endpoint = {row, col};
                    } else {
                        l_gridmap[row][col] = 0;
                    }
                }
                row++;
            }
            file.close();
        } else {
            qDebug() << "Failed to open level_1.txt";
        }
    }
    else {
        initcosts=500;
        Homehp=1;
        MAX_enemies=30;
        for(int i=1;i<=5;i++)spawnQueue.push({2,0});
        for(int i=1;i<=5;i++)spawnQueue.push({2,5});
        for(int i=1;i<=3;i++)spawnQueue.push({1,1});
        for(int i=1;i<=3;i++)spawnQueue.push({1,2});
        for(int i=1;i<=3;i++)spawnQueue.push({1,3});
        for(int i=1;i<=3;i++)spawnQueue.push({1,4});
        for(int i=1;i<=4;i++)spawnQueue.push({3,0});
        for(int i=1;i<=4;i++)spawnQueue.push({3,5});
        for(int i=0;i<=MAP_ROWS;i++)
            for(int j=0;j<=MAP_COLS;j++)
                l_gridmap[i][j]=0;

        QFile file(":/assets/level_5.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            int row = 1;
            while(!in.atEnd() && row <= MAP_ROWS){
                QString line = in.readLine();
                for(int col = 1; col <= MAP_COLS && (col-1) < line.size(); col++){
                    QChar ch = line[col-1];
                    if(ch == '1'){
                        l_gridmap[row][col] = 1;
                    } else if(ch == 'S'){
                        l_gridmap[row][col] = 3;
                        startpoints.push_back({row, col});
                    } else if(ch == 'E'){
                        l_gridmap[row][col] = 4;
                        endpoint = {row, col};
                    } else {
                        l_gridmap[row][col] = 0;
                    }
                }
                row++;
            }
            file.close();
        } else {
            qDebug() << "Failed to open level_1.txt";
        }
    }
}