from bytecore.byte import Byte
from bytecorefast.emulator import ByteCore
from benchmarks.benchmark import Benchmark


class TestBenchmark:
    def test__get_bytecore_assembly_program_that_uses_exactly_100_000_cycles__manually_cycle_until_halt__count_is_equal_to_expected_value(self) -> None:
        # Arrange
        expected_cycles_count = 100_000

        memory_bytes = Benchmark.get_bytecore_assembly_program_that_uses_exactly_100_000_cycles()

        byte_core = ByteCore(memory_bytes)

        # Act
        actual_cycles_count = 0
        temp = expected_cycles_count - 1
        while temp > 0:
            byte_core.cycle()
            temp -= 1
            actual_cycles_count += 1

        assert byte_core.dump().is_halt == Byte(0)
        byte_core.cycle()
        actual_cycles_count += 1
        assert byte_core.dump().is_halt == Byte(1)

        # Assert
        assert actual_cycles_count == expected_cycles_count
