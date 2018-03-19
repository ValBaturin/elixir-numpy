defmodule Test do
    import Numpy

    def copy do
        v = [5.0, 8.5, 10.4, 22.1]
        result = make_list make_vector(v)
        if result != v do
            IO.puts("\nERROR copy")
            IO.puts("input:")
            IO.inspect v
            IO.puts("output")
            IO.inspect result
        end
        IO.puts("✓ copy")
    end

    def deepcopy do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        result = make_deep_list make_matrix(non_sq_matrix)
        if result != non_sq_matrix do
            IO.puts("\n!ERROR deep copy")
            IO.puts("input:")
            IO.inspect non_sq_matrix
            IO.puts("output")
            IO.inspect result
        end
        IO.puts("✓ deep copy")
    end

    def vectorsum do
        v1 = [1.0, 2.0, 5.0]
        v2 = [10.0, 10.0, 10.0]
        result = make_list vsum(make_vector(v1), make_vector(v2))
        if result != [11.0, 12.0, 15.0] do
            IO.puts("\n ERROR vector sum ")
        end
        IO.puts("✓ vector sum")
    end

    def matrixsum do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        non_sq_matrix2 = [[0.0, 1.0, 6.0], [5.5, 3.2, 2.5]]
        result = make_deep_list msum(make_matrix(non_sq_matrix), make_matrix(non_sq_matrix2))
        if result != [[1.0, 3.0, 9.0], [10.5, 6.2, 5.0]] do
            IO.puts("\nERROR matrix sum")
            IO.puts("input:")
            IO.inspect non_sq_matrix
            IO.inspect non_sq_matrix2
            IO.puts("output")
            IO.inspect result
        end
        IO.puts("✓ matrix sum")
    end

    def matrixscale do
        scaler = 15.0
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        result = make_deep_list(scale(scaler, make_matrix(non_sq_matrix)))
        if result != [[15.0, 30.0, 45.0], [75.0, 45.0, 37.5]] do
            IO.puts("\nERROR matrix scale")
            IO.puts("input:")
            IO.inspect scaler
            IO.inspect non_sq_matrix
            IO.puts("output")
            IO.inspect result
        end
        IO.puts("✓ matrix scale")
    end

    def vectorscale do
        scaler = 15.0
        v = [5.0, 3.0, 2.5]
        result = make_list(scale(scaler, make_vector(v)))
        if result != [75.0, 45.0, 37.5] do
            IO.puts("\nERROR vector scale")
            IO.puts("input:")
            IO.inspect scaler
            IO.inspect v
            IO.puts("output")
            IO.inspect result
        end
        IO.puts("✓ vector scale")
    end

    def col do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        col = 0
        result = make_list get_col(col, make_matrix(non_sq_matrix))
        if result != [1.0, 5.0] do
            IO.puts("\nERROR col")
            IO.puts("input:")
            IO.inspect col
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        col = 1
        result = make_list get_col(col, make_matrix(non_sq_matrix))
        if result != [2.0, 3.0] do
            IO.puts("\nERROR col")
            IO.puts("input:")
            IO.inspect col
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        col = 2
        result = make_list get_col(col, make_matrix(non_sq_matrix))
        if result != [3.0, 2.5] do
            IO.puts("\nERROR col")
            IO.puts("input:")
            IO.inspect col
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        IO.puts("✓ get col")
    end

    def row do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        row = 0
        result = make_list get_row(row, make_matrix(non_sq_matrix))
        if result != [1.0, 2.0, 3.0] do
            IO.puts("\n!ERROR row")
            IO.puts("input:")
            IO.inspect row
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        row = 1
        result = make_list get_row(row, make_matrix(non_sq_matrix))
        if result != [5.0, 3.0, 2.5] do
            IO.puts("\n!ERROR row")
            IO.puts("input:")
            IO.inspect row
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        IO.puts("✓ get row")
    end

    def diag do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        d = 1
        result = make_list get_diag(d, make_matrix(non_sq_matrix))
        if result != [1.0, 3.0] do
            IO.puts("\nGET DIAG")
            IO.puts("input:")
            IO.inspect d
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        d = -1
        result = make_list get_diag(d, make_matrix(non_sq_matrix))
        if result != [3.0, 3.0] do
            IO.puts("input:")
            IO.inspect d
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        IO.puts("✓ get diag")
    end

    def rowscale do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        nrow = 0
        row = get_row(nrow, make_matrix(non_sq_matrix))
        scaler = 10.0
        result = make_list(scale(scaler, row))
        if result != [10.0, 20.0, 30.0] do
            IO.puts("\n!ERROR row scale")
        end
        nrow = 1
        row = get_row(nrow, make_matrix(non_sq_matrix))
        scaler = 5.0
        result = make_list(scale(scaler, row))
        if result != [25.0, 15.0, 12.5] do
            IO.puts("\n!ERROR row scale")
        end
        IO.puts("✓ row scale")
    end

    def colscale do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        ncol = 0
        scaler = 10.0
        result = make_list scale(scaler, get_col(ncol, make_matrix(non_sq_matrix)))
        if result != [10.0, 50.0] do
            IO.puts("\nERROR col")
            IO.puts("input:")
            IO.inspect scaler
            IO.inspect ncol
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        ncol = 1
        result = make_list scale(scaler, get_col(ncol, make_matrix(non_sq_matrix)))
        if result != [20.0, 30.0] do
            IO.puts("\nERROR col")
            IO.puts("input:")
            IO.inspect scaler
            IO.inspect ncol
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        ncol = 2
        result = make_list scale(scaler, get_col(ncol, make_matrix(non_sq_matrix)))
        if result != [30.0, 25.0] do
            IO.puts("\nERROR col")
            IO.puts("input:")
            IO.inspect scaler
            IO.inspect ncol
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        IO.puts("✓ col scale")
    end

    def diagscale do
        non_sq_matrix = [[1.0, 2.0, 3.0], [5.0, 3.0, 2.5]]
        d = 1
        scaler = -10.0
        result = make_list scale(scaler, get_diag(d, make_matrix(non_sq_matrix)))
        if result != [-10.0, -30.0] do
            IO.puts("\nGET DIAG")
            IO.puts("input:")
            IO.inspect d
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        d = -1
        result = make_list scale(scaler, get_diag(d, make_matrix(non_sq_matrix)))
        if result != [-30.0, -30.0] do
            IO.puts("input:")
            IO.inspect d
            IO.inspect non_sq_matrix
            IO.puts("output:")
            IO.inspect result
        end
        IO.puts("✓ diag scale")
    end

    def test() do
        copy()
        deepcopy()
        vectorsum()
        matrixsum()
        vectorscale()
        matrixscale()
        col()
        row()
        diag()
        rowscale()
        colscale()
        diagscale()
        IO.puts("DONE")
    end
end
Test.test()
