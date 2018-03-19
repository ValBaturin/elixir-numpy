defmodule Numpy do
    @on_load :init
    def init() do
        :ok = :erlang.load_nif("./numpy", 0)
    end

    def vsum(_, _) do
        "NIF library not loaded"
    end

    def msum(_, _) do
        "NIF library not loaded"
    end

    def vscale(_, _) do
        "NIF library not loaded"
    end

    def make_list(_) do
        "NIF library not loaded"
    end

    def make_deep_list(_) do
        "NIF library not loaded"
    end

    def make_vector(_) do
        "NIF library not loaded"
    end

    def make_matrix(_) do
        "NIF library not loaded"
    end
end
