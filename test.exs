defmodule Test do
    import Numpy
    def deepcopy do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        non_sq_matrix2 = [[0.0, 1.0, 6.0], [5.5, 3.2, 2.5]]
        result = make_deep_list make_matrix(non_sq_matrix)
        IO.puts("\nDEEP COPY")
        IO.puts("input:")
        IO.inspect non_sq_matrix
        IO.puts("output")
        IO.inspect result
    end

    def matrixsum do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        non_sq_matrix2 = [[0.0, 1.0, 6.0], [5.5, 3.2, 2.5]]
        result = msum non_sq_matrix, non_sq_matrix2
        IO.puts("\nMATRIX SUM")
        IO.puts("input:")
        IO.inspect non_sq_matrix
        IO.inspect non_sq_matrix2
        IO.puts("output")
        IO.inspect result
    end

    def matrixscale do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        scaler = 15.0
        result = mscale scaler, non_sq_matrix
        IO.puts("\nMATRIX SUM")
        IO.puts("input:")
        IO.inspect non_sq_matrix
        IO.inspect scaler
        IO.puts("output")
        IO.inspect result
    end

    def test() do
        deepcopy()
        matrixsum()
        matrixscale()
    end
end
Test.test()
