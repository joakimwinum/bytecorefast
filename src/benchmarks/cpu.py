import psutil


class Cpu:
    MEGA: int = 1_000_000

    @staticmethod
    def _get_cpu_speed_in_mhz() -> float:
        return max(psutil.cpu_freq())

    @staticmethod
    def get_cpu_speed_in_hz() -> int:
        return int(Cpu.MEGA * Cpu._get_cpu_speed_in_mhz())
