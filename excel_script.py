import pandas as pd
import sys
import io

def excel(file_name):
    with io.open(file_name, encoding='utf-8') as file:
        content = file.readlines()

    # Извлечение названий строк и столбцов
    rows_names = content[0].strip().split('=')[1].split(',')
    cols_names = content[1].strip().split('=')[1].split(',')

    # Извлечение значений ячеек 
    cells = list(map(float, content[2].strip().split('=')[1].split(',')))

    # Размер таблицы
    num_rows = len(rows_names)
    num_cols = len(cols_names)

    # Преобразуем в DF
    data = []
    for i in range(num_rows):
        row_data = cells[i*num_cols:(i+1)*num_cols]
        data.append(row_data)

    df = pd.DataFrame(data, columns=cols_names, index=rows_names)
    df.to_excel(f'{file_name}.xlsx')
if __name__ == '__main__':
    for i in sys.argv[1:]:
        excel(i)
