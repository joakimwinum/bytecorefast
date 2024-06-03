from bytecore.byte import Byte
from bytecore.memory_bytes_builder import MemoryBytesBuilder
from timeit import Timer


class Benchmark:
    NUMBER_OF_CYCLES: int = 100_000
    NUMBER_OF_REPEATED_TIMINGS: int = 5

    PACKAGE_BYTECORE: str = 'bytecore'
    PACKAGE_BYTECOREFAST: str = 'bytecorefast'

    @staticmethod
    def _get_statement() -> str:
        return "byte_core.cycle_until_halt()"

    @staticmethod
    def _get_setup(package) -> str:
        return f"from benchmarks.benchmark import Benchmark; from {package}.emulator import ByteCore; byte_core = ByteCore(Benchmark.get_bytecore_assembly_program_that_uses_exactly_100_000_cycles())"

    @staticmethod
    def get_bytecore_assembly_program_that_uses_exactly_100_000_cycles() -> list[Byte]:
        memory_bytes = MemoryBytesBuilder()\
            .msb('00').lsb('00').load()\
            .msb('00').lsb('01').data('02')\
            .msb('00').lsb('02').data('00')\
            .msb('00').lsb('03').sub()\
            .msb('00').lsb('04').data('01')\
            .msb('00').lsb('05').data('00')\
            .msb('00').lsb('06').sub()\
            .msb('00').lsb('07').data('01')\
            .msb('00').lsb('08').data('00')\
            .msb('00').lsb('09').add()\
            .msb('00').lsb('0A').data('01')\
            .msb('00').lsb('0B').data('00')\
            .msb('00').lsb('0C').store()\
            .msb('00').lsb('0D').data('02')\
            .msb('00').lsb('0E').data('00')\
            .msb('00').lsb('0F').jz()\
            .msb('00').lsb('10').data('00')\
            .msb('00').lsb('11').data('1E')\
            .msb('00').lsb('12').jmp()\
            .msb('00').lsb('13').data('00')\
            .msb('00').lsb('14').data('15')\
            .msb('00').lsb('15').jmp()\
            .msb('00').lsb('16').data('00')\
            .msb('00').lsb('17').data('18')\
            .msb('00').lsb('18').jmp()\
            .msb('00').lsb('19').data('00')\
            .msb('00').lsb('1A').data('1B')\
            .msb('00').lsb('1B').jmp()\
            .msb('00').lsb('1C').data('00')\
            .msb('00').lsb('1D').data('00')\
            \
            .msb('00').lsb('1E').jmp()\
            .msb('00').lsb('1F').data('00')\
            .msb('00').lsb('20').data('21')\
            .msb('00').lsb('21').jmp()\
            .msb('00').lsb('22').data('00')\
            .msb('00').lsb('23').data('24')\
            .msb('00').lsb('24').jmp()\
            .msb('00').lsb('25').data('00')\
            .msb('00').lsb('26').data('27')\
            .msb('00').lsb('27').jmp()\
            .msb('00').lsb('28').data('00')\
            .msb('00').lsb('29').data('2A')\
            .msb('00').lsb('2A').load()\
            .msb('00').lsb('2B').data('01')\
            .msb('00').lsb('2C').data('01')\
            .msb('00').lsb('2D').store()\
            .msb('00').lsb('2E').data('02')\
            .msb('00').lsb('2F').data('00')\
            .msb('00').lsb('30').load()\
            .msb('00').lsb('31').data('02')\
            .msb('00').lsb('32').data('01')\
            .msb('00').lsb('33').sub()\
            .msb('00').lsb('34').data('01')\
            .msb('00').lsb('35').data('00')\
            .msb('00').lsb('36').store()\
            .msb('00').lsb('37').data('02')\
            .msb('00').lsb('38').data('01')\
            .msb('00').lsb('39').jz()\
            .msb('00').lsb('3A').data('FF')\
            .msb('00').lsb('3B').data('00')\
            .msb('00').lsb('3C').jmp()\
            .msb('00').lsb('3D').data('00')\
            .msb('00').lsb('3E').data('3F')\
            .msb('00').lsb('3F').jmp()\
            .msb('00').lsb('40').data('00')\
            .msb('00').lsb('41').data('42')\
            .msb('00').lsb('42').jmp()\
            .msb('00').lsb('43').data('00')\
            .msb('00').lsb('44').data('45')\
            .msb('00').lsb('45').jmp()\
            .msb('00').lsb('46').data('00')\
            .msb('00').lsb('47').data('00')\
            \
            .msb('01').lsb('00').data('01').comment('  1')\
            .msb('01').lsb('01').data('63').comment(' 99')\
            \
            .msb('02').lsb('00').data('63').comment(' 99')\
            .msb('02').lsb('01').data('64').comment('100')\
            \
            .msb('FF').lsb('00').jmp()\
            .msb('FF').lsb('01').data('FF')\
            .msb('FF').lsb('02').data('03')\
            .msb('FF').lsb('03').jmp()\
            .msb('FF').lsb('04').data('FF')\
            .msb('FF').lsb('05').data('06')\
            .msb('FF').lsb('06').jmp()\
            .msb('FF').lsb('07').data('FF')\
            .msb('FF').lsb('08').data('FF')\
            \
            .msb('FF').lsb('FF').halt()\
            .build()
        return memory_bytes

    @staticmethod
    def _get_timer(package) -> Timer:
        return Timer(Benchmark._get_statement(), setup=Benchmark._get_setup(package))

    @staticmethod
    def _get_execution_time(package) -> list[int]:
        return Benchmark._get_timer(package).timeit(number=1)

    @staticmethod
    def _get_execution_times(package) -> list[float]:
        execution_times: float = []
        for _ in range(Benchmark.NUMBER_OF_REPEATED_TIMINGS):
            execution_times.append(Benchmark._get_execution_time(package))
        return execution_times

    @staticmethod
    def _get_best_execution_time(package) -> float:
        return min(Benchmark._get_execution_times(package))

    @staticmethod
    def _get_number_of_cycles_per_second(package) -> float:
        return int(Benchmark.NUMBER_OF_CYCLES / Benchmark._get_best_execution_time(package))

    @staticmethod
    def get_number_of_cycles_per_second_for_bytecore() -> int:
        return Benchmark._get_number_of_cycles_per_second(Benchmark.PACKAGE_BYTECORE)

    @staticmethod
    def get_number_of_cycles_per_second_for_bytecorefast() -> int:
        return Benchmark._get_number_of_cycles_per_second(Benchmark.PACKAGE_BYTECOREFAST)

    @staticmethod
    def calculate_and_print_performance() -> None:
        bytecore_cycles_per_second = Benchmark.get_number_of_cycles_per_second_for_bytecore()
        bytecorefast_cycles_per_second = Benchmark.get_number_of_cycles_per_second_for_bytecorefast()

        speedup = int(bytecorefast_cycles_per_second /
                      bytecore_cycles_per_second)

        formatted_message = (
            f"Performance Comparison:\n"
            f"ByteCore Emulator: {bytecore_cycles_per_second} cycles/second\n"
            f"ByteCoreFast Emulator: {bytecorefast_cycles_per_second} cycles/second\n"
            f"ByteCoreFast is {speedup} times faster than ByteCore"
        )

        print(formatted_message)
