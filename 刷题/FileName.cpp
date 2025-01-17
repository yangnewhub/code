#include <iostream>
#include <vector>


void Recerseave(std::vector<std::vector<int>>& nums)
{
    int n = nums.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << nums[abs(n - i - 1)][abs(n - j - 1)]<<" ";
        }
        std::cout << std::endl;
    }

}
void Mirror(std::vector<std::vector<int>>& nums)
{
    int n = nums.size();
    for (int i = n-1; i >=0; i--)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << nums[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> nums(n, std::vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cin >> nums[i][j];
        }
    }
    int m, num;
    std::cin >> m;
    while (m--)
    {
        std::cin >> num;
        if (num == 1)  Recerseave(nums);
        else Mirror(nums);
    }
}
