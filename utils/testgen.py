#!/usr/bin/env python3

import io
import click
import os
import sys
import random

@click.group()
def initial():
    pass

def generate_random_graph(num_vertices, num_chunks, max_weight, file=sys.stdout):
    num_edges = random.randrange(num_vertices - 1, num_vertices * (num_vertices - 1) // 2)

    print(num_vertices, num_edges, file=file)
    connected = [0]
    disconnected = list(range(1, num_vertices))
    for i in range(num_vertices - 1):
        source = random.choice(connected)
        destination = random.choice(disconnected) 
        print(source, destination, random.randrange(1, max_weight + 1), file=file)
        connected.append(destination)
        disconnected.remove(destination)
    for i in range(num_vertices - 1, num_edges):
        print(random.randrange(num_vertices), random.randrange(num_vertices), 
                random.randrange(1, max_weight), file=file)

def generate_graph_from_topobench(graph_file, max_edge_weight, file=sys.stdout):
    _, num_vertices, num_edges = graph_file.readline().split()
    num_vertices, num_edges = int(num_vertices), int(num_edges)
    print(num_vertices, num_edges, file=file)
    for i in range(num_edges):
        source, destination, _ = [int(x) for x in graph_file.readline().split()]
        print(source, destination, random.randrange(1, max_edge_weight + 1), file=file)
    return (num_vertices, num_edges)

def generate_chunks(num_vertices, num_chunks, min_chunk_size, max_chunk_size, file=sys.stdout):
    print(num_chunks, file=file)
    for i in range(num_chunks):
        print(random.randrange(1, num_vertices), random.randrange(min_chunk_size, max_chunk_size + 1), file=file)
    print(0, file=file) 

def generate_from_topobench_bytes(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, file=sys.stdout, k = None):
    if k is not None:
        samples = list()
        for _ in range(k):
            graph_file.seek(0)
            samples.append(generate_from_topobench_bytes(
                graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size))
        return samples
    with io.StringIO() as testIO:
        generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, file=testIO)
        return testIO.getvalue().encode('ASCII')

def generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, file=sys.stdout):
    num_vertices,_ = generate_graph_from_topobench(graph_file, max_edge_weight, file=file)
    generate_chunks(num_vertices, num_chunks, min_chunk_size, max_chunk_size, file=file)

def generate_from_topobench_meta(graph_filename, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, output_dir):
    base_name = "cam.nc{:d}.maxe{:d}.minc{:d}.maxc{:d}.txt".format(num_chunks, max_edge_weight, min_chunk_size, max_chunk_size)
    file_name = os.path.join(output_dir, base_name)
    with open(graph_filename) as graph_file, open(file_name, 'w+') as output:
        generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, file=output)

@initial.command()
@click.option('--num-chunks', default=10, help='number of chunks')
@click.option('--max-edge-weight', default=10, help='max weight')
@click.option('--min-chunk-size', default=1, help='min chunk size')
@click.option('--max-chunk-size', default=10, help='max chunk size')
@click.option('--output-dir', help='output directory')
@click.argument('graph_file', type=click.Path(exists=True))
def gen_from_topo(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, output_dir):
    if output_dir is not None:
        generate_from_topobench_meta(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size, output_dir)
    else:
        with open(graph_file) as graph_file:
            generate_from_topobench(graph_file, num_chunks, max_edge_weight, min_chunk_size, max_chunk_size)

if __name__ == '__main__':
    initial()
