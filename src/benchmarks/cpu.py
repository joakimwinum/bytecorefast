import psutil


class Cpu:
    MEGA: int = 1_000_000

    @staticmethod
    def _get_cpu_speed_in_mhz() -> float:
        return max(psutil.cpu_freq())

    @staticmethod
    def is_cpu_speed_available() -> bool:
        try:
            psutil.cpu_freq()
        except RuntimeError as e:
            return False
        return True

    @staticmethod
    def get_cpu_speed_in_hz() -> int:
        return int(Cpu.MEGA * Cpu._get_cpu_speed_in_mhz())
