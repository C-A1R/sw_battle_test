# sw_battle_test

## todo

* проверка на уникальные id
* отрицательные координнаты
* тесты

## По тестовому возникли такие моменты от коллег

* Реализация всех команд лежит в классе Battle еще и в хидере - непонятно почему нельзя было реализацию хотя бы в cpp унести + отдельный класс

* Не поняли зачем тут понадобился темплейт. Такой код не сломается при компиляции, если появится новый тип команды, а значит разработчик забудет его поддержать

    ```c++
    template<typename TCommand> void addCommand(TCommand &&cmd) { }
    template<> void addCommand(io::CreateMap &&cmd)
    ```

* В классе Battle лежит логика по убийству юнитов и атаке, хотя он вроде как должен только тики прокручивать

* Использует туплы вместо отдельной читаемой структуры

    ```c++
    std::tuple<uint32_t, uint32_t, uint32_t> t;
    uint32_t tId {0};
    uint32_t dam {0};
    uint32_t tHp {0};
    std::tie(tId, dam, tHp) = t;
    ```

* Использует unitId = 0 как признак того, что в точке на карте никого нет. Семантически такое решение не говорит другому разработчику, что тут юнита может не быть и что надо не забыть проверить на 0

    ```c++
    _model[it->second.x][it->second.y] = 0
    ```
