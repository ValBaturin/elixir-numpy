defmodule Numpy do
    @on_load :init
    def init() do
        :ok = :erlang.load_nif("./numpy", 0)
    end

    def sum(_, _) do
        "NIF library not loaded"
    end

    def scale(_, _) do
        "NIF library not loaded"
    end
end
