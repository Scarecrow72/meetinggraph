#include <iostream>
#include <string>
#include <exception>

class BadName: public std::exception
{
public:
    virtual const char* what() const noexcept override
    {
        return "Ошибка. Пользователя с таким именем не существует.";
    }
};

class MeetingGraph
{
private:
    static const int m_size {10}; //Определение максимального размера графа
    std::string m_names[m_size];//Массив имен 
    bool m_matrix [m_size][m_size];//Матрица смежности знакомств
    int m_namesCount;//Счетчик добавленных имен

public:
    //Конструктор графа знакомств
    MeetingGraph()
    {
        for(int i = 0; i < m_size; ++i )
            for(int j = 0; j < m_size; ++j)
                m_matrix[i][j] = false;
        m_namesCount = 0;
    }
    //Добавление имени в граф
    void addName(std::string name)
    {
        if(m_namesCount == m_size)
        {
            std::cout << "Ошибка. Память имен переполнена.";
            return;
        }
        m_names[m_namesCount++] = name;
    }
    //Добавление знакомства между двумя людьми в матрицу смежности
    void addMeeting(std::string name1, std::string name2)
    {
        int m {-1};
        int n {-1};
        for(int i = 0; i < m_size; ++i)
        {
            if(name1 == m_names[i])
                m = i;
            if(name2 == m_names[i])
                n = i;
        }
        if(m < 0 || n < 0)
        {
            throw BadName();
            return;
        }
        m_matrix[n][m] = true;
        m_matrix[m][n] = true;
    }
    //Удаления знакомства из матрицы смежности
    void delMeeting(std::string name1, std::string name2)
    {
        int m {-1};
        int n {-1};
        for(int i = 0; i < m_size; ++i)
        {
            if(name1 == m_names[i])
                m = i;
            if(name2 == m_names[i])
                n = i;
        }
        if(m < 0 || n < 0)
        {
            throw BadName();
            return;
        }
        m_matrix[n][m] = false;
        m_matrix[m][n] = false;
    }
    //Удаление имени из списка имен
    void delName(std::string name)
    {
        int match {-1};
        for(int i = 0; i < m_size; ++i)
            if(name == m_names[i])
                match = i;
        if(match < 0)
            throw BadName();
            return;
        for(int i = match; i < m_size - 1; ++i)
        {
            m_matrix[i][match] = m_matrix[i + 1][match];
            m_matrix[match][i] = m_matrix[match][i + 1];
            m_names[i] = m_names[i + 1];
            m_matrix[i][m_size - 1] = false;
            m_matrix[m_size - 1][i] = false;
        }
        m_names[m_size - 1].clear();
        --m_namesCount;
    }
    //Функция определения знакомств через 3 рукопожатия
    void threeHandMeetings(std::string name)
    {
        int match {-1};
        for(int i = 0; i < m_size; ++i)
            if(name == m_names[i])
                match = i;
        if(match < 0)
        {
            throw BadName();
            return;
        }
        std::cout << m_names[match] << " знает:" << std::endl;
        bool visited[m_size];
        for(int i = 0; i < m_size; ++i)
            visited[i] = false;
        int hand {1};
        int stop {3};
        Knowledge(match, visited, hand, stop);
    }
    //Рекурсивная функция
    void Knowledge(int current, bool visited[], int hand, const int& stop)
    {
        if(hand > stop)
            return;
        visited[current] = true;
        for(int i = 0; i < m_size; ++i)
        {
            if(!visited[i] && m_matrix[current][i])
            {
                std::cout << m_names[i] << std::endl;
                Knowledge(i, visited, (hand + 1), stop);
            }
        }
    }
    //Деструктор
    ~MeetingGraph()
    {}
};

int main()
{
    MeetingGraph mgraph;
    
    mgraph.addName("Олег");
    mgraph.addName("Никита");
    mgraph.addName("Настя");
    mgraph.addName("Ваня");
    mgraph.addName("Женя");
    
    mgraph.addMeeting("Олег", "Никита");
    mgraph.addMeeting("Никита", "Настя");
    mgraph.addMeeting("Настя", "Ваня");
    mgraph.addMeeting("Ваня", "Женя");
    
    try
    {
        mgraph.threeHandMeetings("Олег");
        mgraph.threeHandMeetings("Женя");
        mgraph.threeHandMeetings("Настя");
        mgraph.threeHandMeetings("Виктор");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}