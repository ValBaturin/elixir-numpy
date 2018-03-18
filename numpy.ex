defmodule Numpy do
 @on_load :init
  def init() do
    :erlang.load_nif("./numpy", 0)
    :ok
  end

  def sum(_, _) do
    "NIF library not loaded"
  end

end
