import random
import argparse

class stat_data:
    def __init__(self, id, count, cost, primary, mode):
        self.id = id
        self.count = count
        self.cost = cost
        if primary not in (0, 1):
            raise ValueError("primary!!!")
        self.primary = primary
        if not (0 <= mode <= 7):
            raise ValueError("mode!!!")
        self.mode = mode

    def __add__(self, other):
        if not isinstance(other, stat_data):
            return NotImplemented
        if self.id != other.id:
            raise ValueError("Cannot add elements: different IDs!!!")
        # Суммируем count и cost
        new_count = self.count + other.count
        new_cost = self.cost + other.cost
        # primary будет равен 1 только если оба primary равны 1, иначе 0
        new_primary = 1 if (self.primary == 1 and other.primary == 1) else 0
        # mode берём как максимум
        new_mode = max(self.mode, other.mode)
        return stat_data(self.id, new_count, new_cost, new_primary, new_mode)

    def __repr__(self):
        return (f"stat_data(id={self.id}, count={self.count}, cost={self.cost}, "
                f"primary={self.primary}, mode={self.mode})")


class custom_dict(dict):
    def __setitem__(self, key, value):
        if key in self:
            current_value = super().__getitem__(key)
            new_value = current_value + value
            super().__setitem__(key, new_value)
        else:
            super().__setitem__(key, value)

def generate_test_data(suffix: str, n: int, start_id: int = 0):
    base_name = f"stat_data_test_{suffix}"
    filename = f"{base_name}.h"
    include_guard = f"{base_name.upper()}_H"
    array_name = f"case_{suffix}_in"

    array_in_a = dict()
    array_in_b = dict()
    array_out = custom_dict()

    with open(filename, "w") as f:
        f.write(f"#ifndef {include_guard}\n")
        f.write(f"#define {include_guard}\n\n")
        f.write('#include "stat_data.h"\n\n')

        # Первый массив
        f.write(f"static stat_data {array_name}_a[{n}] = {{\n")
        for i in range(n):
            id_value = start_id + i
            count = random.randint(0, 1000)
            cost = round(random.uniform(0, 10000), 3)
            primary = random.randint(0, 1)
            mode = random.randint(0, 7)
            array_in_a[id_value] = stat_data(id_value, count, cost, primary, mode)
            f.write("    {{{}, {}, {:.3f}f, {}, {}}}".format(id_value, count, cost, primary, mode))
            if i < n - 1:
                f.write(",\n")
            else:
                f.write("\n")
        f.write("};\n\n")

        # Второй массив
        f.write(f"static stat_data {array_name}_b[{n}] = {{\n")
        for i in range(n):
            id_value = n + start_id + i - 5
            count = random.randint(0, 1000)
            cost = round(random.uniform(0, 10000), 3)
            primary = random.randint(0, 1)
            mode = random.randint(0, 7)
            array_in_b[id_value] = stat_data(id_value, count, cost, primary, mode)
            f.write("    {{{}, {}, {:.3f}f, {}, {}}}".format(id_value, count, cost, primary, mode))
            if i < n - 1:
                f.write(",\n")
            else:
                f.write("\n")
        f.write("};\n\n")

        # Итоговый массив
        for el in array_in_a:
            array_out[el] = array_in_a[el]
        for el in array_in_b:
            array_out[el] = array_in_b[el]
        sorted_items = sorted(array_out.items(), key=lambda item: item[1].cost)
        array_out = {k: v for k, v in sorted_items}

        f.write(f"static stat_data case_{suffix}_out[{len(array_out)}] = {{\n")
        i = 0
        for el in array_out:
            f.write("    {{{}, {}, {:.3f}f, {}, {}}}".format(array_out[el].id,
                                                             array_out[el].count,
                                                             array_out[el].cost,
                                                             array_out[el].primary,
                                                             array_out[el].mode))
            if i < len(array_out) - 1:
                f.write(",\n")
            else:
                f.write("\n")
            i = i + 1
        f.write("};\n\n")

        f.write(f"#endif // {include_guard}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate stat_data test header.")
    parser.add_argument("suffix", help="Suffix for filename and array name (e.g., '1' for stat_data_test_1.h)")
    parser.add_argument("count", type=int, help="Number of elements to generate")
    parser.add_argument("--start-id", type=int, default=0, help="Starting value for the 'id' field (default: 0)")

    args = parser.parse_args()
    generate_test_data(args.suffix, args.count, args.start_id)

